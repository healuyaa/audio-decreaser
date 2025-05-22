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
        const char* window_icon = "../resource/window_icon32.png";

        const char* glsl_version = "#version 330";
        ImGuiIO io;

        void initWindow();
        void initUInterface();
        void loopUInterface();
        void renderUInterface();

        Interface interface{};

        void initFont();
    };
}