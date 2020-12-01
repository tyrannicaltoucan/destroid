#pragma once

#include <glad/gl.h>
#include <SDL.h>
#include <memory>
#include <string>

namespace destroid {

class Window {
public:
    explicit Window(const std::string& title, int width, int height);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void enableVerticalSync(bool toggle) const;
    void swapFramebuffers() const;

private:
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_windowHandle;
    SDL_GLContext m_glContext;
};

} // namespace destroid
