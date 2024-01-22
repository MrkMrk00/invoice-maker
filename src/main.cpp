#include "imgui.h"
#include "main_window.hpp"

static struct {
    bool display_demo = false;
} DRAW_CTX{};

static inline void draw_main_menu() {
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Nápověda")) {
        if (ImGui::MenuItem("Zobrazit Demo", NULL, DRAW_CTX.display_demo)) {
            DRAW_CTX.display_demo = !DRAW_CTX.display_demo;
        }

        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
}

int main(int, char **) {
    auto app = invoice_maker::main_window_create("Fakturník cpp",
                                                 {.x = 1280, .y = 720});

    app->load_font_utf8("./assets/Inter.ttf", 16.f);

    invoice_maker::DrawOptions opts{
        .clear_color = {0.45f, 0.55f, 0.60f, 1.00f},
    };

    invoice_maker::window_loop(*app, opts, [&]() {
        draw_main_menu();
        if (DRAW_CTX.display_demo) {
            ImGui::ShowDemoWindow();
        }
    });

    return 0;
}
