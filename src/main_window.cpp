#include "main_window.hpp"

#include <GLFW/glfw3.h>

#include <cstdint>
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

static bool GLFW_INITD = false;
static const char* GLSL_VERSION;
static uint8_t GL_VERSION_MINOR;
static uint8_t GL_VERSION_MAJOR;

static int glfw_init() {
    glfwSetErrorCallback([](int error, const char* descr) {
        fprintf(stderr, "GLFW Error %d: %s\n", error, descr);
    });

    if (!glfwInit()) {
        return 1;
    }

#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    GLSL_VERSION = "#version 100";
    GL_VERSION_MAJOR = 2;
    GL_VERSION_MINOR = 0;
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    GLSL_VERSION = "#version 150";
    GL_VERSION_MAJOR = 3;
    GL_VERSION_MINOR = 2;
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  // Required on Mac
#else
    // GL 3.0 + GLSL 130
    GLSL_VERSION = "#version 130";
    GL_VERSION_MAJOR = 3;
    GL_VERSION_MINOR = 0;
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
    // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
    GLFW_INITD = true;
    return 0;
}

static ImWchar IMFONT_UTF_RANGE[3] = {0x20, 0xFFFF, 0};

namespace invoice_maker {

InvoiceMaker::~InvoiceMaker() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(this->main_window);
    glfwTerminate();
}

int InvoiceMaker::load_font_utf8(const char* file_path, float font_size) {
    auto& io = ImGui::GetIO();
    (void)io;

    auto font = io.Fonts->AddFontFromFileTTF(file_path, font_size, NULL,
                                             IMFONT_UTF_RANGE);

    IM_ASSERT(font != nullptr);
    this->fonts.push_back(font);

    return this->fonts.size() - 1;
}

std::unique_ptr<InvoiceMaker> main_window_create(const char* window_title,
                                                 WindowSize size) {
    if (!GLFW_INITD) {
        int err = glfw_init();
        if (err > 0) {
            fprintf(stderr, "Failed to initialize GLFW\n");
            exit(err);
        }
    }

    GLFWwindow* window =
        glfwCreateWindow(size.x, size.y, window_title, nullptr, nullptr);

    if (window == nullptr) {
        fprintf(stderr, "Failed to create GLFW Window\n");
        exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |=
        ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // ImGui::StyleColorsDark();
    ImGui::StyleColorsLight();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);

    auto main_window = std::make_unique<InvoiceMaker>(window);

    return main_window;
}

void window_loop(const InvoiceMaker& app, const DrawOptions& opts,
                 std::function<void()> loop_func) {
    auto window = app.main_window;
    auto clear_color = opts.clear_color;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        loop_func();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w,
                     clear_color.y * clear_color.w,
                     clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
}

}  // namespace invoice_maker
