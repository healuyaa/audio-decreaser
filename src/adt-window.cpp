#include "adt-window.hpp"

#include "adt-icon.hpp"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <string>

namespace adt {
    Window::Window(int w, int h, std::string name) : width(w), height(h), window_name(name) {
        initWindow();
        initUInterface();
        initFont();
    }

    Window::~Window() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Window::loopUInterface() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void Window::renderUInterface() {
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    void Window::runUInterface() {
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
            return;
        }

        loopUInterface();

        interface.UInterface();
        
        renderUInterface();
    }

    void Window::initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, window_name.c_str(), nullptr, nullptr);

        int w, h, ch;

        unsigned char* icon = stbi_load(window_icon, &w, &h, &ch, 4);
        if (icon) {
            GLFWimage img;
            img.width = w;
            img.height = h;
            img.pixels = icon;
            glfwSetWindowIcon(window, 1, &img);
            stbi_image_free(icon);
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
        }
    }

    void Window::initUInterface() {
        IMGUI_CHECKVERSION();

        ImGui::CreateContext();
        io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsLight();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
    }

    void Window::initFont() {
        io = ImGui::GetIO();

        io.Fonts->AddFontFromFileTTF("../assets/fonts/Lorenzo Sans Regular.ttf", 18.0f);

        io.FontDefault = io.Fonts->Fonts.back();
    }
}
