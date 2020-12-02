#include "game.hpp"
#include "component/drawable.hpp"
#include "component/velocity.hpp"
#include "component/player.hpp"
#include "component/transform.hpp"
#include "system/drawing.hpp"
#include "system/input.hpp"
#include "system/physics.hpp"
#include <glm/vec4.hpp>
#include <random>

namespace destroid {

namespace {

    constexpr glm::vec4 BUFFER_CLEAR_COLOR{ 0.F, 0.F, 0.F, 1.F };
    constexpr Rectangle GAME_BOUNDS{ 0.F, 0.F, 450.F, 300.F };
    constexpr int ASTEROID_COUNT = 5;

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

    const Rectangle asteroidRegion { glm::vec2{ 32.F, 0.F }, glm::vec2 { 32.F } };
    const float maxAsteroidSpeed = 45.F;
    glm::vec2 asteroidPosition { 0.F, 0.F };

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> spawnDirection(1, 4);
    std::uniform_real_distribution<float> velocity(-maxAsteroidSpeed, maxAsteroidSpeed);
    std::uniform_real_distribution<float> posX(GAME_BOUNDS.x, GAME_BOUNDS.width);
    std::uniform_real_distribution<float> posY(GAME_BOUNDS.y, GAME_BOUNDS.height);

    for (auto i = 0; i < ASTEROID_COUNT; i++) {
        const entt::entity asteroid = m_registry.create();

        switch(spawnDirection(rng)) {
        case 1:
            asteroidPosition = glm::vec2{ GAME_BOUNDS.x, posY(rng) };
            break;
        case 2:
            asteroidPosition = glm::vec2{ GAME_BOUNDS.width, posY(rng) };
            break;
        case 3:
            asteroidPosition = glm::vec2{ posX(rng), GAME_BOUNDS.y };
            break;
        case 4:
            asteroidPosition = glm::vec2{ posX(rng), GAME_BOUNDS.height };
            break;
        }

        m_registry.emplace<Drawable>(asteroid, m_debugTexture, asteroidRegion);
        m_registry.emplace<Velocity>(asteroid, glm::vec2{ velocity(rng), velocity(rng) });
        m_registry.emplace<Transform>(asteroid, asteroidPosition);
    }
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
