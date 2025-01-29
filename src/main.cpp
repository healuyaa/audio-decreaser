#include "run-app.hpp"

#include <iostream>
#include <exception>

int main(int argc, char** argv) {
    adt::App app{};

    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
