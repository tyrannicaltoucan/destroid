#include "window.hpp"
#include <SDL.h>
#include <glad/gl.h>
#include <stdexcept>

namespace destroid {

void Window::WinDeleter::operator()(SDL_Window* window)
{
    SDL_DestroyWindow(window);
}

void Window::WinDeleter::operator()(SDL_GLContext context)
{
    SDL_GL_DeleteContext(context);
}

Window::Window(const std::string& title, int width, int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error("SDL2 initialization failure: " + std::string(SDL_GetError()));
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, SDL_TRUE);

    m_window = std::unique_ptr<SDL_Window, WinDeleter>(SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_OPENGL));

    if (m_window == nullptr) {
        throw std::runtime_error("Window creation failure: " + std::string(SDL_GetError()));
    }

    m_context = std::unique_ptr<void, WinDeleter>(SDL_GL_CreateContext(m_window.get()));

    if (m_context == nullptr) {
        throw std::runtime_error("OpenGL context failure: " + std::string(SDL_GetError()));
    }

    SDL_GL_SetSwapInterval(1);
    SDL_ShowCursor(SDL_FALSE);

    if (gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress)) == 0) {
        throw std::runtime_error("OpenGL context failure: Unable to load OpenGL function pointers");
    }
}

Window::~Window()
{
    SDL_Quit();
}

void Window::present()
{
    SDL_GL_SwapWindow(m_window.get());
}

} // namespace destroid
