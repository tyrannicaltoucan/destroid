#pragma once

#include "core/window.hpp"
#include "graphics/renderer.hpp"
#include "graphics/texture.hpp"
#include <entt/entity/registry.hpp>

namespace destroid {

class Game {
public:
    Game();

    void run();

private:
    void processEvents();
    void update(float delta);
    void render();

    Window m_window;
    Renderer m_renderer;
    entt::registry m_registry;
    Texture m_spriteSheet;
    bool m_running = true;
};

} // namespace destroid
