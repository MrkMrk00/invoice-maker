cmake_minimum_required(VERSION 3.13)

project(invoice_maker LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLFW REQUIRED glfw3)

set(IMGUI_DIR vendor/imgui)

set(IMGUI_SOURCES
	${IMGUI_DIR}/imgui.cpp 
	${IMGUI_DIR}/imgui_demo.cpp 
	${IMGUI_DIR}/imgui_draw.cpp 
	${IMGUI_DIR}/imgui_tables.cpp 
	${IMGUI_DIR}/imgui_widgets.cpp
)

set(IMGUI_GLFW_SOURCES
	${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
	${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_executable(invoice_maker
	src/main.cpp
	${IMGUI_SOURCES}
	${IMGUI_GLFW_SOURCES}
)

target_link_libraries(invoice_maker 
	${OPENGL_LIBRARY}
	${GLFW_LIBRARIES}
)

target_include_directories(invoice_maker PUBLIC 
	${GLFW_INCLUDE_DIRS}
	${IMGUI_DIR}
	${IMGUI_DIR}/backends
)

target_compile_options(invoice_maker PUBLIC ${GLFW_CFLAGS_OTHER})

