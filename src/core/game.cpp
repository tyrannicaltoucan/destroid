#include "game.hpp"
#include "entity/factory.hpp"
#include "math/rectangle.hpp"
#include "systems/collision.hpp"
#include "systems/despawn.hpp"
#include "systems/drawing.hpp"
#include "systems/expiry.hpp"
#include "systems/input.hpp"
#include "systems/movement.hpp"
#include "systems/spawning.hpp"
#include "systems/weapon.hpp"
#include <SDL.h>
#include <chrono>

namespace destroid {
namespace {

constexpr int windowWidth = 900;
constexpr int windowHeight = 600;
constexpr float virtualWidth = windowWidth / 2.0f;
constexpr float virtualHeight = windowHeight / 2.0f;

} // namespace

Game::Game()
    : m_window("Destroid", windowWidth, windowHeight)
    , m_spriteSheet("assets/sprites.png")
{
    m_renderer.setViewBounds(virtualWidth, virtualHeight);
    m_registry.set<Rect>(Rect(0.0f, 0.0f, virtualWidth, virtualHeight));

    entity::createShip(m_registry);
    entity::createSpawner(m_registry);
}

void Game::run()
{
    using Clock = std::chrono::steady_clock;
    using Duration = std::chrono::duration<float>;
    using TimePoint = std::chrono::time_point<Clock, Duration>;

    TimePoint previousTime = Clock::now();

    while (m_running) {
        const TimePoint currentTime = Clock::now();
        const Duration delta = currentTime - previousTime;
        previousTime = currentTime;

        processEvents();
        update(delta.count());
        render();

        m_window.present();
    }
}

void Game::processEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
        case SDL_QUIT:
            m_running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                m_running = false;
            }
            break;
        }
    }
}

void Game::update(float delta)
{
    input_system::update(m_registry, SDL_GetKeyboardState(nullptr), delta);
    spawning_system::update(m_registry);
    movement_system::update(m_registry, delta);
    weapon_system::update(m_registry, delta);
    collision_system::update(m_registry);
    expiry_system::update(m_registry, delta);
    despawn_system::update(m_registry);
}

void Game::render()
{
    m_renderer.clearColor(0.0f, 0.0f, 0.0f);
    drawing_system::update(m_registry, m_renderer, m_spriteSheet);
    m_renderer.finish();
}

} // namespace destroid
