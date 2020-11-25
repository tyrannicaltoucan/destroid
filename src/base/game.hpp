#pragma once

#include "graphics/renderer.hpp"
#include "graphics/texture.hpp"
#include <entt/entity/registry.hpp>
#include <glm/vec2.hpp>
#include <SDL.h>
#include <memory>

namespace destroid {

class Game {
public:
    Game();

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void pollInput(const unsigned char* keystate);
    void update(float delta);
    void draw(Renderer& renderer);

private:
    std::shared_ptr<Texture> m_debugTexture;
    entt::registry m_registry;
};

} // namespace destroid
