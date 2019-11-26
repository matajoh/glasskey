#include "frame.h"
#include <GL/glut.h>
#include <GL/freeglut_ext.h>

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
Frame::Frame(std::uint16_t rows, std::uint16_t cols, const std::string &title) : m_rows(rows),
                                                                                 m_cols(cols),
                                                                                 m_title(title),
                                                                                 m_running(false),
                                                                                 m_width(cols*COLW),
                                                                                 m_height(rows*ROWH)
{
    for (auto r = 0; r < rows; ++r)
    {
        m_grid.emplace_back(cols, ' ');
    }
}

Frame::~Frame()
{
    stop();
}

std::unique_ptr<Frame> Frame::m_instance = nullptr;

Frame &Frame::Init(std::uint16_t rows, std::uint16_t cols, const std::string &title)
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

char Frame::get(std::uint16_t row, std::uint16_t col) const
{
    return m_grid[row][col];
}

const std::string &Frame::get(std::uint16_t row) const
{
    return m_grid[row];
}

Frame &Frame::set(std::uint16_t row, std::uint16_t col, const std::string &values)
{
    std::lock_guard<std::mutex> guard(m_rows_mutex);
    std::copy(values.begin(), values.end(), m_grid[row].begin() + col);
    return *this;
}

void Frame::draw_rows()
{
    std::lock_guard<std::mutex> guard(m_rows_mutex);
    float y = 0;
    for (auto &row : m_grid)
    {
        glRasterPos2f(0, y);
        for (auto &col : row)
        {
            glutBitmapCharacter(GLUT_BITMAP_9_BY_15, col);
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
    glColor3d(1.0, 0.0, 0.0);
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