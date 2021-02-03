#pragma once

#include "graphics/texture.hpp"
#include <entt/entity/registry.hpp>

namespace destroid {

class Renderer;

class GameScene {
public:
    GameScene();
    ~GameScene() = default;

    void processInput(const unsigned char* keystate, float delta);
    void update(float delta);
    void draw(Renderer& renderer);

private:
    entt::registry m_registry;
    Texture m_spriteSheet;
};

} // namespace destroid
