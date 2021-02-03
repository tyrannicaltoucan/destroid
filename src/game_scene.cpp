#include "game_scene.hpp"
#include "base/rectangle.hpp"
#include "entity/factory.hpp"
#include "graphics/renderer.hpp"
#include "system/collision.hpp"
#include "system/despawn.hpp"
#include "system/drawing.hpp"
#include "system/expiry.hpp"
#include "system/input.hpp"
#include "system/movement.hpp"
#include "system/spawning.hpp"
#include "system/weapon.hpp"
#include <glm/vec4.hpp>

namespace destroid {

namespace {

    constexpr auto GAME_BOUNDS = Rectangle(0.F, 0.F, 450.F, 300.F);

} // namespace

GameScene::GameScene()
    : m_spriteSheet("assets/sprites.png")
{
    m_registry.set<Rectangle>(GAME_BOUNDS);
    entity_factory::spawnPlayer(m_registry);
    entity_factory::createSpawner(m_registry);
}

void GameScene::processInput(const unsigned char* keystate, float delta)
{
    input_system::update(m_registry, keystate, delta);
}

void GameScene::update(float delta)
{
    spawning_system::update(m_registry);
    despawn_system::update(m_registry);
    movement_system::update(m_registry, delta);
    weapon_system::update(m_registry, delta);
    collision_system::update(m_registry);
    expiry_system::update(m_registry, delta);
}

void GameScene::draw(Renderer& renderer)
{
    renderer.clearColor({0.F, 0.F, 0.F, 1.F});
    renderer.setViewBounds(GAME_BOUNDS.width, GAME_BOUNDS.height);
    drawing_system::update(m_registry, renderer, m_spriteSheet);
    renderer.finish();
}

} // namespace destroid
