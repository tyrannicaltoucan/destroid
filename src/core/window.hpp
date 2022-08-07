#pragma once

#include <SDL_video.h>
#include <memory>
#include <string>

namespace destroid {

class Window {
public:
    Window(const std::string& title, int width, int height);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    void present();

private:
    struct WinDeleter {
        void operator()(SDL_Window* window);
        void operator()(SDL_GLContext context);
    };

    std::unique_ptr<SDL_Window, WinDeleter> m_window;
    std::unique_ptr<void, WinDeleter> m_context;
};

} // namespace destroid
