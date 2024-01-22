#include <iostream>

#include "database.hpp"
#include "imgui.h"
#include "main_window.hpp"

namespace DB = invoice_maker::DB;

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

void db_init() {
    bool users_exists = DB::table_exists("Users");

    std::cout << __PRETTY_FUNCTION__ << " :: "
              << (users_exists ? "Table Users exists"
                               : "Table Users does not exist")
              << std::endl;

    if (!users_exists) {
        std::cout << "Creating Users table" << std::endl;

        auto q_users = DB::query(R"(CREATE TABLE IF NOT EXISTS Users (
            id      BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
            name    VARCHAR(255) NOT NULL UNIQUE
        ))");

        q_users->next();
    }
}

int main(int, char **) {
    auto app = invoice_maker::main_window_create("Fakturník cpp",
                                                 {.x = 1280, .y = 720});
    DB::connect();
    db_init();

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

    DB::destroy();

    return 0;
}
