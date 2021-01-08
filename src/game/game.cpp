#include "game.hpp"
#include "entity/factory.hpp"
#include "system/collision.hpp"
#include "system/despawn.hpp"
#include "system/drawing.hpp"
#include "system/input.hpp"
#include "system/movement.hpp"
#include "system/weapon.hpp"
#include <glad/gl.h>
#include <SDL.h>
#include <chrono>
#include <memory>
#include <random>
#include <stdexcept>

namespace destroid {

namespace {

    using Clock = std::chrono::steady_clock;
    using Duration = std::chrono::duration<float>;

    constexpr const char* DEFAULT_TITLE = "Destroid";
    constexpr int DEFAULT_WIDTH = 900;
    constexpr int DEFAULT_HEIGHT = 600;
    constexpr Rectangle GAME_BOUNDS{0.F, 0.F, DEFAULT_WIDTH / 2.F, DEFAULT_HEIGHT / 2.F};
    constexpr int ASTEROID_COUNT = 5;

    SDL_Window* makeWindow()
    {
        const unsigned int flags = SDL_WINDOW_OPENGL;

        auto window = SDL_CreateWindow(
            DEFAULT_TITLE,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            DEFAULT_WIDTH,
            DEFAULT_HEIGHT,
            flags);

        if (!window) {
            const auto error = std::string(SDL_GetError());
            throw std::runtime_error("Failed to create window: " + error);
        }

        return window;
    }

    SDL_GLContext makeOpenGLContext(SDL_Window* window)
    {
        auto context = SDL_GL_CreateContext(window);

        if (gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress)) == 0) {
            throw std::runtime_error("Unable to create OpenGL context!");
        }

        return context;
    }

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
    : m_window(nullptr, SDL_DestroyWindow)
    , m_glContext(nullptr, SDL_GL_DeleteContext)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        const auto error = std::string(SDL_GetError());
        throw std::runtime_error("Failed to initialize SDL2: " + error);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    m_window.reset(makeWindow());
    m_glContext.reset(makeOpenGLContext(m_window.get()));
    m_renderer = std::make_unique<Renderer>();
    m_spriteAtlas = std::make_unique<Texture>("assets/sprites.png");
    SDL_GL_SetSwapInterval(1);
    m_active = true;

    m_registry.set<Rectangle>(GAME_BOUNDS);
    entity_factory::createPlayer(m_registry);
    createAsteroidField(m_registry);
}

void Game::run()
{
    auto previousTime = Clock::now();

    while (m_active) {
        const auto currentTime = Clock::now();
        const auto delta = Duration(currentTime - previousTime);
        previousTime = currentTime;

        pollEvents();
        update(delta.count());
        draw();
    }

    SDL_Quit();
}

void Game::pollEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
        case SDL_QUIT:
            m_active = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                m_active = false;
            }
            break;
        default:
            break;
        }
    }
}

void Game::update(float delta)
{
    const auto* keystate = SDL_GetKeyboardState(nullptr);

    movement_system::update(m_registry, delta);
    collision_system::update(m_registry);
    weapon_system::update(m_registry, delta);
    despawn_system::update(m_registry, delta);
    input_system::update(m_registry, keystate, delta);
}

void Game::draw()
{
    m_renderer->clearColor({0.F, 0.F, 0.F, 1.F});
    m_renderer->setViewBounds(GAME_BOUNDS.width, GAME_BOUNDS.height);
    drawing_system::update(m_registry, *m_renderer, *m_spriteAtlas);
    m_renderer->finish();

    SDL_GL_SwapWindow(m_window.get());
}

} // namespace destroid
