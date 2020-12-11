#include "game.hpp"
#include "component/drawable.hpp"
#include "component/velocity.hpp"
#include "component/player.hpp"
#include "component/transform.hpp"
#include "component/collider.hpp"
#include "component/asteroid.hpp"
#include "system/drawing.hpp"
#include "system/input.hpp"
#include "system/physics.hpp"
#include <system/collision.hpp>
#include <glm/vec4.hpp>
#include <random>

namespace destroid {

namespace {

    constexpr glm::vec4 BUFFER_CLEAR_COLOR{0.F, 0.F, 0.F, 1.F};
    constexpr Rectangle GAME_BOUNDS{0.F, 0.F, 450.F, 300.F};
    constexpr int ASTEROID_COUNT = 5;

    void createPlayer(entt::registry& registry, std::shared_ptr<Texture> texture)
    {
        const Rectangle region{0, 0, 32, 32};
        const glm::vec2 position = GAME_BOUNDS.center() - region.center();
        const Circle bounds{position, region.width * 0.25F};
        const float thrustSpeed = 10.F;
        const float rotationSpeed = thrustSpeed * 0.25F;
        const float damping = 0.95F;

        const entt::entity player = registry.create();
        registry.emplace<Transform>(player, position);
        registry.emplace<Velocity>(player);
        registry.emplace<Player>(player, thrustSpeed, rotationSpeed, damping);
        registry.emplace<Collider>(player, bounds);
        registry.emplace<Drawable>(player, texture, region);
    }

    void createAsteroidField(entt::registry& registry, std::shared_ptr<Texture> texture)
    {
        std::random_device rd;
        std::minstd_rand gen(rd());

        std::uniform_real_distribution<float> xPos(0, GAME_BOUNDS.width);
        std::uniform_real_distribution<float> yPos(0, GAME_BOUNDS.height);
        std::uniform_real_distribution<float> speed(-45, 45);
        std::uniform_int_distribution<int> spawnDir(1, 4);

        const Rectangle region{32, 0, 32, 32};
        glm::vec2 position{};

        for (int i = 0; i < ASTEROID_COUNT; i++) {
            switch (spawnDir(gen)) {
            case 1: // left
                position = {GAME_BOUNDS.x, yPos(gen)};
                break;
            case 2: // right
                position = {GAME_BOUNDS.width, yPos(gen)};
                break;
            case 3: // bottom
                position = {xPos(gen), GAME_BOUNDS.y};
                break;
            case 4: // top
                position = {xPos(gen), GAME_BOUNDS.height};
                break;
            }

            const glm::vec2 velocity{speed(gen), speed(gen)};
            const Circle bounds{position, region.width * 0.25F};
            const entt::entity asteroid = registry.create();
            registry.emplace<Transform>(asteroid, position);
            registry.emplace<Velocity>(asteroid, velocity);
            registry.emplace<Asteroid>(asteroid);
            registry.emplace<Collider>(asteroid, bounds);
            registry.emplace<Drawable>(asteroid, texture, region);
        }
    }

} // namespace

Game::Game()
    : m_debugTexture(std::make_shared<Texture>("assets/sprites.png"))
{
    m_registry.set<Rectangle>(GAME_BOUNDS);

    createPlayer(m_registry, m_debugTexture);
    createAsteroidField(m_registry, m_debugTexture);
}

void Game::pollInput(const unsigned char* keystate)
{
    input_system::update(m_registry, keystate);
}

void Game::update(float delta)
{
    collision_system::update(m_registry);
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
