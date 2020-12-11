#include "base/application.hpp"
#include <SDL.h>
#include <exception>
#include <iostream>

void showErrorDialog(const char* message)
{
    if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Critical Error", message, nullptr) != 0) {
        std::cerr << message << '\n';
    }
}

int main(int argc, char* argv[])
{
    try {
        destroid::Application app{"Destroid", 900, 600};
        app.run();
    } catch (const std::exception& e) {
        showErrorDialog(e.what());
        return EXIT_FAILURE;
    } catch (...) {
        showErrorDialog("Encountered unknown error!");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
