cmake_minimum_required(VERSION 3.13)

# Setup backend independent Dear ImGUI
set(IMGUI_DIR imgui)
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

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_DIR})


# ImGUI with statically linked glfw3 and dynamic OpenGL
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GLFW REQUIRED glfw3)

add_library(imgui_glfw STATIC ${IMGUI_GLFW_SOURCES})
target_link_libraries(imgui_glfw 
	imgui
	${OPENGL_LIBRARY}
	${GLFW_LIBRARIES}
)

target_include_directories(imgui_glfw PUBLIC
	${GLFW_INCLUDE_DIRS}
	${IMGUI_DIR}/backends
)

target_compile_options(imgui_glfw PUBLIC ${GLFW_CFLAGS_OTHER})

