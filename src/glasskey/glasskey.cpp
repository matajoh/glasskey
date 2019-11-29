#include "glasskey.h"

#include <atomic>
#include <thread>

#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <atomic>
#include <map>
#include <queue>
#include <thread>

namespace
{
bool g_init = false;
std::atomic_bool g_running = false;
std::thread g_main_thread;
std::mutex g_grid_mutex;
std::map<int, std::shared_ptr<gk::TextGrid>> g_grid_map;
std::queue<std::shared_ptr<gk::TextGrid>> g_to_create;
int g_start_x = 10;
const int COLW = 9;
const int ROWH = 15;
} // namespace

namespace gk
{
void init(const std::vector<std::string> &args)
{
    if (!g_init)
    {
        int argc = static_cast<int>(args.size());
        std::vector<char *> argv;
        std::transform(args.begin(), args.end(), std::back_inserter(argv),
                       [](const std::string &arg) -> char * { return const_cast<char *>(arg.c_str()); });
        glutInit(&argc, argv.data());
        g_init = true;
    }
}

std::shared_ptr<TextGrid> create_grid(Size rows, Size cols, const std::string &title, const Color &default_color)
{
    std::lock_guard<std::mutex> guard(g_grid_mutex);
    auto text_grid = std::make_shared<TextGrid>(TextGrid(rows, cols, title, default_color));
    g_to_create.push(text_grid);
    return text_grid;
}

void destroy_grid(std::shared_ptr<TextGrid> text_grid)
{
    std::lock_guard<std::mutex> guard(g_grid_mutex);
    glutDestroyWindow(text_grid->id());
    g_grid_map.erase(text_grid->id());
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

void resize(int width, int height)
{
    std::lock_guard<std::mutex> guard(g_grid_mutex);
    auto text_grid = g_grid_map[glutGetWindow()];
    width = text_grid->cols() * COLW;
    height = text_grid->rows() * ROWH;
    const float ar = (float)width / (float)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display()
{
    std::lock_guard<std::mutex> guard(g_grid_mutex);
    auto text_grid = g_grid_map[glutGetWindow()];
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    set_orthographic_projection(text_grid->cols()*COLW, text_grid->rows()*ROWH);
    glPushMatrix();
    glLoadIdentity();
    text_grid->draw_rows();
    glPopMatrix();
    reset_perspective_projection();
    glutSwapBuffers();
}

void close()
{
    std::lock_guard<std::mutex> guard(g_grid_mutex);
    g_grid_map[glutGetWindow()]->id() = -1;
    g_grid_map.erase(glutGetWindow());
}

void create_grids()
{
    std::lock_guard<std::mutex> guard(g_grid_mutex);
    while(g_to_create.size()){
        auto text_grid = g_to_create.front();
        glutInitWindowSize(text_grid->cols()*COLW, text_grid->rows()*ROWH);
        glutInitWindowPosition(g_start_x, 10);
        g_start_x += text_grid->cols()*COLW;
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
        text_grid->id() = glutCreateWindow(text_grid->title().c_str());
        glutReshapeFunc(&resize);
        glutDisplayFunc(&display);
        glutCloseFunc(&close);
        g_grid_map[text_grid->id()] = text_grid;
        g_to_create.pop();
    }
}

void refresh()
{
    std::lock_guard<std::mutex> guard(g_grid_mutex);
    for(auto& pair : g_grid_map)
    {
        if(pair.second->is_dirty()){
            glutSetWindow(pair.first);
            glutPostRedisplay();
        }
    }
}

void main_loop()
{
    init();
    while(g_running.load()){
        {
            create_grids();
        }

        refresh();
        glutMainLoopEvent();
    }
}

void start()
{
    if(!g_running.load()){
        g_running.store(true);
        g_main_thread = std::thread(&main_loop);
    }
}

void stop()
{
    if(g_running.load()){
        g_running.store(false);
        g_main_thread.join();
    }
}

} // namespace gk