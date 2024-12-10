#pragma once

#include "adt-interface.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui.h"

#include <string>

namespace adt {
    class Window {
        public:
        Window(int w, int h, std::string name);
        ~Window();

        bool ShouldClose() { return glfwWindowShouldClose(window); }

        void runUInterface();

        private:
        GLFWwindow* window;

        const int width;
        const int height;
        const std::string window_name;

        const char* glsl_version = "#version 330";
        ImGuiIO io;

        void initWindow();
        void initUInterface();
        void loopUInterface();
        void renderUInterface();

        Interface interface{};
    };
}