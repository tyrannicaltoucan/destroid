#include "game.hpp"
#include "component/drawable.hpp"
#include "component/velocity.hpp"
#include "component/player.hpp"
#include "component/transform.hpp"
#include "system/drawing.hpp"
#include "system/input.hpp"
#include "system/physics.hpp"
#include <glm/vec4.hpp>

namespace destroid {

namespace {

    constexpr glm::vec4 BUFFER_CLEAR_COLOR{ 0.F, 0.F, 0.F, 1.F };
    constexpr Rectangle GAME_BOUNDS{ 0.F, 0.F, 450.F, 300.F };

} // namespace

Game::Game()
    : m_debugTexture(std::make_shared<Texture>("assets/sprites.png"))
{
    m_registry.set<Rectangle>(GAME_BOUNDS);

    const float movementSpeed = 10.F;
    const float rotationSpeed = 2.5F;
    const float damping = 0.95F;
    const Rectangle playerRegion{ glm::vec2{ 0.F }, glm::vec2{ 32.F } };
    const glm::vec2 position = GAME_BOUNDS.center() - playerRegion.center();

    const entt::entity player = m_registry.create();
    m_registry.emplace<Drawable>(player, m_debugTexture, playerRegion);
    m_registry.emplace<Player>(player, movementSpeed, rotationSpeed, damping);
    m_registry.emplace<Velocity>(player);
    m_registry.emplace<Transform>(player, position);
}

void Game::pollInput(const unsigned char* keystate)
{
    input_system::update(m_registry, keystate);
}

void Game::update(float delta)
{
    physics_system::update(m_registry, delta);
}

void Game::draw(Renderer& renderer)
{
    renderer.clearColor(BUFFER_CLEAR_COLOR);
    renderer.setViewBounds(GAME_BOUNDS.width, GAME_BOUNDS.height);
    drawing_system::update(m_registry, renderer);
    renderer.finish();
}

} // namespace destroid
