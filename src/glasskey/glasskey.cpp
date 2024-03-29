#include "glasskey/glasskey.h"

#include <algorithm>
#include <atomic>
#include <iostream>
#include <thread>
#include <queue>

#include <GL/freeglut_std.h>
#include <GL/freeglut_ext.h>


namespace
{
bool g_init = false;
std::atomic_bool g_is_running = false;
std::thread g_main_thread;
std::mutex g_grid_mutex;
std::map<int, std::shared_ptr<gk::TextGrid>> g_grid_map;
std::queue<std::shared_ptr<gk::TextGrid>> g_to_create;
std::queue<std::shared_ptr<gk::TextGrid>> g_to_destroy;
std::chrono::time_point<std::chrono::system_clock> g_last_refresh_time;
int g_start_x = 10;
std::mutex g_pressed_mutex;
std::map<gk::Key, bool> g_is_pressed;
std::map<unsigned char, gk::Key> g_key_map = {
    {32, gk::Key::SPACE},
    {13, gk::Key::ENTER}
};
std::map<int, gk::Key> g_special_map = {
    {GLUT_KEY_UP, gk::Key::UP},
    {GLUT_KEY_DOWN, gk::Key::DOWN},
    {GLUT_KEY_RIGHT, gk::Key::RIGHT},
    {GLUT_KEY_LEFT, gk::Key::LEFT}
};
} // namespace

namespace gk
{

std::ostream &operator<<(std::ostream &os, const Rect &rect)
{
    return os << rect.to_string();
}

std::ostream &operator<<(std::ostream &os, const Color &color)
{
    return os << color.to_string();
}

std::ostream &operator<<(std::ostream &os, const Letter &letter)
{
    return os << letter.to_string();
}

std::ostream &operator<<(std::ostream &os, const TextGrid &grid)
{
    return os << grid.to_string();
}

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
    g_to_destroy.push(text_grid);
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
    auto text_grid = g_grid_map[glutGetWindow()];
    width = text_grid->cols() * COL_WIDTH;
    height = text_grid->rows() * ROW_HEIGHT;
    const float ar = (float)width / (float)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void display_grid()
{
    auto text_grid = g_grid_map[glutGetWindow()];
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    set_orthographic_projection(text_grid->cols() * COL_WIDTH, text_grid->rows() * ROW_HEIGHT);
    glPushMatrix();
    glLoadIdentity();
    text_grid->draw_rows();
    glPopMatrix();
    reset_perspective_projection();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    if(g_key_map.count(key)){
        std::lock_guard<std::mutex> guard(g_pressed_mutex);
        g_is_pressed[g_key_map[key]] = true;
    }
}

void keyboardup(unsigned char key, int x, int y)
{
    if(g_key_map.count(key)){
        std::lock_guard<std::mutex> guard(g_pressed_mutex);
        g_is_pressed[g_key_map[key]] = false;
    }
}

void special(int key, int x, int y)
{
    if(g_special_map.count(key)){
        std::lock_guard<std::mutex> guard(g_pressed_mutex);
        g_is_pressed[g_special_map[key]] = true;
    }
}

void specialup(int key, int x, int y)
{
    if(g_special_map.count(key)){
        std::lock_guard<std::mutex> guard(g_pressed_mutex);
        g_is_pressed[g_special_map[key]] = false;
    }    
}

bool is_pressed(Key key)
{
    std::lock_guard<std::mutex> guard(g_pressed_mutex);
    return g_is_pressed[key];
}

void close()
{
    std::lock_guard<std::mutex> guard(g_grid_mutex);
    g_grid_map[glutGetWindow()]->id() = -1;
    g_grid_map.erase(glutGetWindow());
}

void create_and_destroy_grids()
{
    std::lock_guard<std::mutex> guard(g_grid_mutex);
    while (g_to_create.size())
    {
        auto text_grid = g_to_create.front();
        glutInitWindowSize(text_grid->cols() * COL_WIDTH, text_grid->rows() * ROW_HEIGHT);
        glutInitWindowPosition(g_start_x, 10);
        g_start_x += text_grid->cols() * COL_WIDTH;
        glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
        glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
        text_grid->id() = glutCreateWindow(text_grid->title().c_str());
        glutReshapeFunc(resize);
        glutDisplayFunc(display_grid);
        glutKeyboardFunc(keyboard);
        glutKeyboardUpFunc(keyboardup);
        glutSpecialFunc(special);
        glutSpecialUpFunc(specialup);
        glutCloseFunc(close);
        g_grid_map[text_grid->id()] = text_grid;
        g_to_create.pop();
    }

    while (g_to_destroy.size())
    {
        auto text_grid = g_to_destroy.front();
        glutDestroyWindow(text_grid->id());
        g_grid_map.erase(text_grid->id());
        g_to_destroy.pop();
    }
}

void refresh_grids()
{
    for (auto &pair : g_grid_map)
    {
        if (pair.second->is_dirty())
        {
            glutSetWindow(pair.first);
            glutPostRedisplay();
        }
    }
}

void main_loop()
{
    init();
    auto lastFrameTime = std::chrono::system_clock::now();
    while (g_is_running.load())
    {
        create_and_destroy_grids();
        refresh_grids();
        glutMainLoopEvent();
    }
}

void start()
{
    if (!g_is_running.load())
    {
        g_is_running.store(true);
        g_main_thread = std::thread(&main_loop);
    }
}

void stop()
{
    if (g_is_running.load())
    {
        g_is_running.store(false);
        g_main_thread.join();
    }
}

void next_frame(float frames_per_second)
{
    auto time_per_frame = std::chrono::milliseconds(static_cast<std::size_t>(1000.0f/frames_per_second));
    auto duration = std::chrono::system_clock::now() - g_last_refresh_time;
    if(duration < time_per_frame){
        std::this_thread::sleep_for(time_per_frame - duration);
    }

    g_last_refresh_time = std::chrono::system_clock::now();
}

} // namespace gk