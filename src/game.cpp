#include "game.hpp"
#include <glad/gl.h>
#include <SDL.h>
#include <chrono>
#include <stdexcept>

namespace destroid {

namespace {

    using Clock = std::chrono::steady_clock;
    using Duration = std::chrono::duration<float>;

    constexpr const char* DEFAULT_TITLE = "Destroid";
    constexpr int DEFAULT_WIDTH = 900;
    constexpr int DEFAULT_HEIGHT = 600;

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

    m_window.reset(SDL_CreateWindow(
        DEFAULT_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DEFAULT_WIDTH,
        DEFAULT_HEIGHT,
        SDL_WINDOW_OPENGL));

    if (!m_window) {
        const std::string error = SDL_GetError();
        throw std::runtime_error("Failed to create window: " + error);
    }

    m_glContext.reset(SDL_GL_CreateContext(m_window.get()));
    if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress))) {
        throw std::runtime_error("Failed to load OpenGL funciton symbols");
    }

    SDL_GL_SetSwapInterval(1);
    m_renderer = std::make_unique<Renderer>();
    m_scene = std::make_unique<GameScene>();
}

void Game::run()
{
    bool active = true;
    auto previousTime = Clock::now();

    while (active) {
        const auto currentTime = Clock::now();
        const auto delta = Duration(currentTime - previousTime);
        previousTime = currentTime;

        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
            case SDL_QUIT:
                active = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    active = false;
                }
            default:
                break;
            }
        }

        m_scene->processInput(SDL_GetKeyboardState(nullptr), delta.count());
        m_scene->update(delta.count());
        m_scene->draw(*m_renderer);

        SDL_GL_SwapWindow(m_window.get());
    }

    SDL_Quit();
}

} // namespace destroid
