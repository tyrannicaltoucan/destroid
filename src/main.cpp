#include "core/game.hpp"
#include <SDL_messagebox.h>
#include <cstdlib>
#include <exception>
#include <iostream>

int main(int argc, char* argv[])
{
    static_cast<void>(argc);
    static_cast<void>(argv);

    try {
        auto game = destroid::Game();
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
