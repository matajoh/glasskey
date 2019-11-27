#include "text_grid.h"
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <iostream>
#include <functional>

namespace
{
const int COLW = 9;
const int ROWH = 15;
bool g_init = false;
std::mutex g_init_mutex;
std::condition_variable g_init_cv;
} // namespace

namespace gk
{
Letter::Letter() : Letter(' ', Colors::White){}

Letter::Letter(char value, const Color &color) : value(value), color(color) {}

Row::Row(Size cols, const Color& color) : std::vector<Letter>(cols)
{
    std::fill(begin(), end(), Letter(' ', color));
}

TextGrid::TextGrid(Size rows, Size cols, const std::string &title, const Color &default_color) : m_rows(rows),
                                                                                 m_cols(cols),
                                                                                 m_title(title),
                                                                                 m_running(false),
                                                                                 m_width(cols*COLW),
                                                                                 m_height(rows*ROWH),
                                                                                 m_default_color(default_color)
{
    for (auto r = 0; r < rows; ++r)
    {
        m_grid.emplace_back(cols);
    }
}

TextGrid::~TextGrid()
{
    stop();
}

std::unique_ptr<TextGrid> TextGrid::m_instance = nullptr;

TextGrid &TextGrid::Init(Size rows, Size cols, const std::string &title)
{
    TextGrid::m_instance = std::make_unique<TextGrid>(rows, cols, title);
    return TextGrid::Instance();
}

TextGrid &TextGrid::Instance()
{
    return *TextGrid::m_instance;
}

void TextGrid::start()
{
    if (!m_running.load())
    {
        m_running.store(true);
        m_main_thread = std::thread(&TextGrid::main_loop);
        {
            std::unique_lock<std::mutex> lock(g_init_mutex);
            g_init_cv.wait(lock, []{return g_init;});
        }
    }
}

void TextGrid::stop()
{
    if (m_running.load())
    {
        m_running.store(false);
        m_main_thread.join();
    }
}

const Letter &TextGrid::get(Index row, Index col) const
{
    return m_grid[row][col];
}

const Row &TextGrid::get(Index row) const
{
    return m_grid[row];
}

TextGrid &TextGrid::set(Index row, Index col, const std::string &values)
{
    std::lock_guard<std::mutex> guard(m_rows_mutex);
    if(row < 0){
        row += m_rows;
    }

    Index left = fix_range(col, m_cols);
    Index right = fix_range(col + Size(values.size()), m_cols);
    auto first = values.begin() + (left - col);
    auto last = first + (right - left);
    std::vector<Letter> letters;
    std::transform(first, last, std::back_inserter(letters),
    [this](char value)->Letter{return Letter(value, get_color(value));});
    std::copy(letters.begin(), letters.end(), m_grid[row].begin() + left);
    return *this;
}

TextGrid &TextGrid::set(Index row, Index col, const std::vector<Letter> &letters)
{
    std::lock_guard<std::mutex> guard(m_rows_mutex);
    if(row < 0){
        row += m_rows;
    }

    Index left = fix_range(col, m_cols);
    Index right = fix_range(col + Size(letters.size()), m_cols);
    auto first = letters.begin() + (left - col);
    auto last = first + (right - left);
    std::copy(first, last, m_grid[row].begin() + left);
    return *this;
}

TextGrid &TextGrid::set(const Rect & rect, char value)
{
    std::lock_guard<std::mutex> guard(m_rows_mutex);
    Letter letter(value, get_color(value));
    Rect clip = rect.clip(m_cols, m_rows);
    if(clip.area() == 0){
        return *this;
    }

    auto top = m_grid.begin() + clip.top();
    auto bottom = m_grid.begin() + clip.bottom();
    for(auto row = top; row < bottom; ++row)
    {
        auto left = row->begin() + clip.left();
        auto right = row->begin() + clip.right();
        std::fill(left, right, letter);
    }
    return *this;
}

TextGrid &TextGrid::clear(Index row, Index col, Size cols)
{
    std::lock_guard<std::mutex> guard(m_rows_mutex);
    Index left = fix_range(col, m_cols);
    Index right = fix_range(col + cols, m_cols);
    if(right - left == 0){
        return *this;
    }

    auto first = m_grid[row].begin() + left;
    auto last = m_grid[row].begin() + right;
    std::fill(first, last, Letter());
    return *this;
}

TextGrid &TextGrid::clear(const Rect& rect)
{
    return set(rect, ' ');
}

TextGrid &TextGrid::map_color(char value, const Color &color)
{
    m_color_map[value] = color;
    return *this;
}

TextGrid &TextGrid::unmap_color(char value)
{
    m_color_map.erase(value);
    return *this;
}

const Color&TextGrid::get_color(char value) const
{
    if(m_color_map.count(value)){
        return m_color_map.at(value);
    }

    return m_default_color;
}

void TextGrid::draw_rows()
{
    std::lock_guard<std::mutex> guard(m_rows_mutex);
    float y = 15;
    for (auto &row : m_grid)
    {
        float x = 0;
        for (auto &letter : row)
        {
            if(letter.value == ' '){
                x += COLW;
                continue;
            }

            glColor3f(letter.color.r, letter.color.g, letter.color.b);
            glRasterPos2f(x, y);
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, letter.value);
            x += COLW;
        }

        y += ROWH;
    }
}

void TextGrid::refresh()
{
    if(m_running.load()){
        glutPostRedisplay();
    }
}

void set_orthographic_projection(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glScalef(1, -1, 1);
    glTranslatef(0, static_cast<GLfloat>(-h), 0);
    glMatrixMode(GL_MODELVIEW);
}

void reset_perspective_projection()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void TextGrid::resize(int width, int height)
{
    const float ar = (float) width / (float) height;
    TextGrid::m_instance->m_width = width;
    TextGrid::m_instance->m_height = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
} 

void TextGrid::display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    set_orthographic_projection(TextGrid::m_instance->m_width, TextGrid::m_instance->m_height);
    glPushMatrix();
    glLoadIdentity();
    TextGrid::m_instance->draw_rows();
    glPopMatrix();
    reset_perspective_projection();
    glutSwapBuffers();
}

Size TextGrid::cols() const
{
    return m_cols;
}

Size TextGrid::rows() const
{
    return m_rows;
}

void TextGrid::close()
{
    TextGrid::m_instance->m_running.store(false);
}

void TextGrid::main_loop()
{
    {
        std::lock_guard<std::mutex> lock(g_init_mutex);
        int argc = 1;
        char *argv[1] = {"dummy.exe"};
        glutInit(&argc, argv);
        g_init = true;
        glutInitWindowSize(TextGrid::m_instance->m_width, TextGrid::m_instance->m_height);
        glutInitWindowPosition(10, 10);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
        glutCreateWindow(TextGrid::m_instance->m_title.c_str());
        glutReshapeFunc(&TextGrid::resize);
        glutDisplayFunc(&TextGrid::display);
        glutCloseFunc(&TextGrid::close);
    }
    g_init_cv.notify_one();

    while (TextGrid::m_instance->m_running.load())
    {
        glutMainLoopEvent();
    }
}
} // namespace gk