#include "window.hpp"
#include <stdexcept>

namespace destroid {

namespace {

    const std::string sdlError = std::string(SDL_GetError());

    SDL_Window* createWindow(const char* title, int width, int height)
    {
        unsigned int flags = SDL_WINDOW_OPENGL;

        SDL_Window* windowHandle = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            flags);

        if (!windowHandle) {
            throw std::runtime_error("Failed to create window: " + sdlError);
        }

        return windowHandle;
    }

    SDL_GLContext createGLContext(SDL_Window* windowHandle)
    {
        SDL_GLContext glContext = SDL_GL_CreateContext(windowHandle);

        if (!glContext) {
            throw std::runtime_error("OpenGL initialization error: " + sdlError);
        }

        if (gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress)) == 0) {
            throw std::runtime_error("Failed to load OpenGL function symbols");
        }

        return glContext;
    }

} // namespace

Window::Window(const std::string& title, int width, int height)
    : m_windowHandle(nullptr, SDL_DestroyWindow)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error("Failed to initialize SDL2: " + sdlError);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    m_windowHandle.reset(createWindow(title.c_str(), width, height));
    m_glContext = createGLContext(m_windowHandle.get());
}

Window::~Window()
{
    SDL_GL_DeleteContext(m_glContext);
    SDL_Quit();
}

void Window::enableVerticalSync(bool toggle) const
{
    const int vsyncMode = toggle ? 1 : 0;
    SDL_GL_SetSwapInterval(vsyncMode);
}

void Window::swapFramebuffers() const
{
    SDL_GL_SwapWindow(m_windowHandle.get());
}

} // namespace destroid
