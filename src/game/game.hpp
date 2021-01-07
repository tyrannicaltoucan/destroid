#pragma once

#include "graphics/renderer.hpp"
#include "graphics/texture.hpp"
#include <entt/entity/fwd.hpp>
#include <entt/entity/registry.hpp>
#include <SDL_video.h>
#include <memory>

namespace destroid {

class Game {
public:
    Game();
    ~Game() = default;

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void run();

private:
    void pollEvents();
    void update(float delta);
    void draw();

    std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_window;
    std::unique_ptr<void, decltype(&SDL_GL_DeleteContext)> m_glContext;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Texture> m_spriteAtlas;
    entt::registry m_registry;
    bool m_active = false;
};

}
