#include "run-app.hpp"

namespace adt {
    void App::run() {
        while(!window.ShouldClose()) {
            glfwPollEvents();
            window.runUInterface();
        }
    }   
}