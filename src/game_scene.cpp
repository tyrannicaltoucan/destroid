#include "game_scene.hpp"
#include "factory/entity_factory.hpp"
#include "graphics/renderer.hpp"
#include "math/rectangle.hpp"
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

constexpr auto viewBounds = Rectangle(0.0f, 0.0f, 450.0f, 300.0f);

} // namespace

GameScene::GameScene()
    : m_spriteSheet("assets/sprites.png")
{
    m_registry.set<Rectangle>(viewBounds);
    entity_factory::createShip(m_registry);
    entity_factory::createSpawner(m_registry);
}

void GameScene::processInput(const unsigned char* keystate, float delta)
{
    input_system::update(m_registry, keystate, delta);
}

void GameScene::update(float delta)
{
    spawning_system::update(m_registry);
    movement_system::update(m_registry, delta);
    weapon_system::update(m_registry, delta);
    collision_system::update(m_registry);
    expiry_system::update(m_registry, delta);
    despawn_system::update(m_registry);
}

void GameScene::draw(Renderer& renderer)
{
    renderer.clearColor({0.0f, 0.0f, 0.0f, 1.0f});
    renderer.setViewBounds(viewBounds.width, viewBounds.height);
    drawing_system::update(m_registry, renderer, m_spriteSheet);
    renderer.finish();
}

} // namespace destroid
