#include "game.hpp"
#include "entity/factory.hpp"
#include "entity/systems.hpp"
#include <glm/vec4.hpp>
#include <random>

namespace destroid {

namespace {

    constexpr glm::vec4 BUFFER_CLEAR_COLOR{0.F, 0.F, 0.F, 1.F};
    constexpr Rectangle GAME_BOUNDS{0.F, 0.F, 450.F, 300.F};
    constexpr int ASTEROID_COUNT = 5;

    void createAsteroidField(entt::registry& reg)
    {
        std::random_device rd;
        std::minstd_rand gen(rd());

        std::uniform_real_distribution<float> xPos(0, GAME_BOUNDS.width);
        std::uniform_real_distribution<float> yPos(0, GAME_BOUNDS.height);
        std::uniform_real_distribution<float> speed(-45, 45);
        std::uniform_int_distribution<int> spawnDir(1, 4);

        glm::vec2 position;

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
            entity_factory::createAsteroid(reg, position, velocity);
        }
    }

} // namespace

Game::Game()
    : m_debugTexture(std::make_shared<Texture>("assets/sprites.png"))
{
    m_registry.set<Rectangle>(GAME_BOUNDS);
    m_registry.set<std::shared_ptr<Texture>>(m_debugTexture);

    entity_factory::createPlayer(m_registry);
    createAsteroidField(m_registry);

    m_registry.unset<std::shared_ptr<Texture>>();
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
