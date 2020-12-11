#include "application.hpp"
#include "game.hpp"
#include <chrono>

namespace destroid {

Application::Application(const std::string& title, int windowWidth, int windowHeight)
    : m_window(title, windowWidth, windowHeight)
{
    m_window.enableVerticalSync(true);
}

void Application::run()
{
    auto previousTime = std::chrono::steady_clock::now();

    Game game;
    bool active = true;

    while (active) {
        SDL_Event event;

        auto currentTime = std::chrono::steady_clock::now();
        float delta = std::chrono::duration<float>(currentTime - previousTime).count();
        previousTime = currentTime;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                active = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    active = false;
                }
                break;
            }
        }

        const unsigned char* keystate = SDL_GetKeyboardState(nullptr);
        game.pollInput(keystate);

        game.update(delta);
        game.draw(m_renderer);

        m_window.swapFramebuffers();
    }
}

} // namespace destroid
