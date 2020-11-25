#pragma once

#include "game.hpp"
#include "window.hpp"
#include "graphics/renderer.hpp"
#include <string>

namespace destroid {

class Application {
public:
    Application(const std::string& title, int windowWidth, int windowHeight);

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void run();

private:
    Window m_window;
    Renderer m_renderer;
};

} // namespace destroid
