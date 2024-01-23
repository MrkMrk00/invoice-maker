#ifndef _H_MAIN_WINDOW
#define _H_MAIN_WINDOW

#include <GLFW/glfw3.h>

#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <functional>
#include <memory>

#include "imgui.h"
namespace invoice_maker {

struct DrawOptions {
    ImVec4 clear_color;
};

class MainWindow {
   private:
    GLFWwindow *window;
    std::vector<ImFont *> fonts;

   public:
    explicit MainWindow(GLFWwindow *window_) : window(window_) {}
    ~MainWindow();

    [[nodiscard]] const GLFWwindow *get_window() const { return this->window; }
    [[nodiscard]] ImFont *get_font(const int index) const {
        return this->fonts.at(index);
    }
    size_t load_font_utf8(const char *file_path, float font_size);

    friend void window_loop(const MainWindow &app, const DrawOptions &opts,
                            const std::function<void()> &loop_func);
};

struct WindowSize {
    unsigned int x, y;
};

std::unique_ptr<MainWindow> main_window_create(const char *window_title,
                                               WindowSize size);

void window_loop(const MainWindow &app, const DrawOptions &opts,
                 const std::function<void()> &loop_func);

}  // namespace invoice_maker

#endif  // !_H_MAIN_WINDOW
