#include "frame.h"
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

Row::Row(Index cols, const Color& color) : std::vector<Letter>(cols)
{
    std::fill(begin(), end(), Letter(' ', color));
}

Frame::Frame(Index rows, Index cols, const std::string &title, const Color &default_color) : m_rows(rows),
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

Frame::~Frame()
{
    stop();
}

std::unique_ptr<Frame> Frame::m_instance = nullptr;

Frame &Frame::Init(Index rows, Index cols, const std::string &title)
{
    Frame::m_instance = std::make_unique<Frame>(rows, cols, title);
    return Frame::Instance();
}

Frame &Frame::Instance()
{
    return *Frame::m_instance;
}

void Frame::start()
{
    if (!m_running.load())
    {
        m_running.store(true);
        m_main_thread = std::thread(&Frame::main_loop);
        {
            std::unique_lock<std::mutex> lock(g_init_mutex);
            g_init_cv.wait(lock, []{return g_init;});
        }
    }
}

void Frame::stop()
{
    if (m_running.load())
    {
        m_running.store(false);
        m_main_thread.join();
    }
}

const Letter &Frame::get(Index row, Index col) const
{
    return m_grid[row][col];
}

const Row &Frame::get(Index row) const
{
    return m_grid[row];
}

Frame &Frame::set(Index row, Index col, const std::string &values)
{
    std::lock_guard<std::mutex> guard(m_rows_mutex);
    std::vector<Letter> letters;
    std::transform(values.begin(), values.end(), std::back_inserter(letters),
    [this](char value)->Letter{return Letter(value, get_color(value));});
    std::copy(letters.begin(), letters.end(), m_grid[row].begin() + col);
    return *this;
}

Frame &Frame::set(Index row, Index col, const std::vector<Letter> &letters)
{
    std::lock_guard<std::mutex> guard(m_rows_mutex);
    std::copy(letters.begin(), letters.end(), m_grid[row].begin() + col);
    return *this;
}

Frame &Frame::set(const Rect & rect, char value)
{
    std::lock_guard<std::mutex> guard(m_rows_mutex);
    Letter letter(value, get_color(value));
    auto top = m_grid.begin() + rect.top;
    auto bottom = m_grid.begin() + rect.bottom;
    for(auto row = top; row < bottom; ++row)
    {
        auto left = row->begin() + rect.left;
        auto right = row->begin() + rect.right;
        std::fill(left, right, letter);
    }
    return *this;
}

Frame &Frame::clear(Index row, Index col, Index cols)
{
    std::lock_guard<std::mutex> guard(m_rows_mutex);
    auto first = m_grid[row].begin() + col;
    auto last = first + cols;
    std::fill(first, last, Letter());
    return *this;
}

Frame &Frame::clear(const Rect& rect)
{
    return set(rect, ' ');
}

Frame &Frame::map_color(char value, const Color &color)
{
    m_color_map[value] = color;
    return *this;
}

Frame &Frame::unmap_color(char value)
{
    m_color_map.erase(value);
    return *this;
}

const Color&Frame::get_color(char value) const
{
    if(m_color_map.count(value)){
        return m_color_map.at(value);
    }

    return m_default_color;
}

void Frame::draw_rows()
{
    std::lock_guard<std::mutex> guard(m_rows_mutex);
    float y = 0;
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

void Frame::refresh()
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

void Frame::resize(int width, int height)
{
    const float ar = (float) width / (float) height;
    Frame::m_instance->m_width = width;
    Frame::m_instance->m_height = height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
} 

void Frame::display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    set_orthographic_projection(Frame::m_instance->m_width, Frame::m_instance->m_height);
    glPushMatrix();
    glLoadIdentity();
    Frame::m_instance->draw_rows();
    glPopMatrix();
    reset_perspective_projection();
    glutSwapBuffers();
}

void Frame::close()
{
    Frame::m_instance->m_running.store(false);
}

void Frame::main_loop()
{
    {
        std::lock_guard<std::mutex> lock(g_init_mutex);
        int argc = 1;
        char *argv[1] = {"dummy.exe"};
        glutInit(&argc, argv);
        g_init = true;
        glutInitWindowSize(Frame::m_instance->m_width, Frame::m_instance->m_height);
        glutInitWindowPosition(10, 10);
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
        glutCreateWindow(Frame::m_instance->m_title.c_str());
        glutReshapeFunc(&Frame::resize);
        glutDisplayFunc(&Frame::display);
        glutCloseFunc(&Frame::close);
    }
    g_init_cv.notify_one();

    while (Frame::m_instance->m_running.load())
    {
        glutMainLoopEvent();
    }
}
} // namespace gk