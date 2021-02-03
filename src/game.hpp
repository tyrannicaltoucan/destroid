#pragma once

#include "game_scene.hpp"
#include "graphics/renderer.hpp"
#include <SDL_video.h>
#include <memory>

namespace destroid {

class Game {
public:
    Game();
    ~Game() = default;

    void run();

private:
    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window;
    std::unique_ptr<void, decltype(&SDL_GL_DeleteContext)> m_glContext;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<GameScene> m_scene;
};

} // namespace destroid
