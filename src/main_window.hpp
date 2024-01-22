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

class InvoiceMaker {
   private:
    GLFWwindow *main_window;
    std::vector<ImFont *> fonts;

   public:
    InvoiceMaker(GLFWwindow *main_window_) : main_window(main_window_) {}
    ~InvoiceMaker();

    const GLFWwindow *window() const { return this->main_window; }
    ImFont *get_font(int index) { return this->fonts.at(index); }
    int load_font_utf8(const char *file_path, float font_size);

    friend void window_loop(const InvoiceMaker &app, const DrawOptions &opts,
                            std::function<void()> loop_func);
};

struct WindowSize {
    unsigned int x, y;
};

std::unique_ptr<InvoiceMaker> main_window_create(const char *window_title,
                                                 WindowSize size);

void window_loop(const InvoiceMaker &app, const DrawOptions &opts,
                 std::function<void()> loop_func);

}  // namespace invoice_maker

#endif  // !_H_MAIN_WINDOW
