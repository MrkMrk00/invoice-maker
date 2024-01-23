#include "data.hpp"
#include "database.hpp"
#include "imgui.h"
#include "main_window.hpp"
#include "ui.h"

namespace DB = invoice_maker::DB;

static struct {
    bool display_demo = false;
    bool bank_accounts = false;
} DRAW_CTX{};

static inline void draw_main_menu() {
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Nápověda")) {
        if (ImGui::MenuItem("Zobrazit Demo", NULL, DRAW_CTX.display_demo)) {
            DRAW_CTX.display_demo = !DRAW_CTX.display_demo;
        }

        ImGui::EndMenu();
    }
    if (ImGui::MenuItem("Bankovní účty", NULL, DRAW_CTX.bank_accounts)) {
        DRAW_CTX.bank_accounts = !DRAW_CTX.bank_accounts;
    }
    ImGui::EndMainMenuBar();
}

struct BankAccountsState {
    bool display_add = false;
    char number[255] = {0};
};

static inline void BankAccounts(bool &visible) {
    static invoice_maker::ui::State<BankAccountsState> bank_accounts_state{};
    auto state = bank_accounts_state.use();

    if (!visible) {
        return;
    }

    ImGui::Begin("Bankovní účty", &visible);
    if (ImGui::Button("+ Přidat", {75, 30})) {
        state()->display_add = true;
    }
    ImGui::End();

    if (state()->display_add) {
        ImGui::Begin("Přidat bankovní účet", &state()->display_add);
        ImGui::InputText("Číslo", state()->number, 255);
        ImGui::End();
    }
}

int main(int, char **) {
    auto app = invoice_maker::main_window_create("Fakturník cpp",
                                                 {.x = 1280, .y = 720});
    DB::connect();
    invoice_maker::data::migrate();

    app->load_font_utf8("./assets/Inter.ttf", 16.f);

    invoice_maker::DrawOptions opts{
        .clear_color = {0.45f, 0.55f, 0.60f, 1.00f},
    };

    invoice_maker::window_loop(*app, opts, [&]() {
        draw_main_menu();
        if (DRAW_CTX.display_demo) {
            ImGui::ShowDemoWindow();
        }
        BankAccounts(DRAW_CTX.bank_accounts);
    });

    DB::destroy();

    return 0;
}
