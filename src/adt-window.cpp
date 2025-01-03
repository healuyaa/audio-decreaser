#include "adt-window.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <iostream>
#include <string>

namespace adt {
    Window::Window(int w, int h, std::string name) : width(w), height(h), window_name(name) {
        initWindow();
        initUInterface();
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

        {
            ImGui::Begin("stats");

            io = ImGui::GetIO();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

            ImGui::End();
        }

        renderUInterface();
    }

    void Window::initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, window_name.c_str(), nullptr, nullptr);

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
}









// ImGui::Image((void*)(intptr_t)iconTexture, ImVec2(64.0f, 64.0f));
// ImGui::SameLine();
// ImGui::SetCursorPosY((64.0f - ImGui::GetFontSize() - 30.0f) / 2.0f);
// ImGui::Text("compresed-test-song.wav");

// ImGui::SameLine();

// ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 32.0f - 5.0f);
// ImGui::SetCursorPosY(32.0f);
// if(ImGui::InvisibleButton("##iconButton", ImVec2(32.0f, 32.0f))) {
//     nfdchar_t *outPath = NULL;
//     nfdresult_t result = NFD_PickFolder(NULL, &outPath);

//     if(result == NFD_OKAY) {
//         std::cout << "path: " << outPath << std::endl;
//     } else {
//         std::cerr << "err\n";
//     }
// }

// ImGui::SetCursorScreenPos(ImGui::GetItemRectMin());
// ImGui::Image((void*)(intptr_t)iconTexture_, ImVec2(32.0f, 32.0f));