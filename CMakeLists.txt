cmake_minimum_required(VERSION 3.13)

project(invoice_maker LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE debug)

add_subdirectory(vendor)

add_executable(invoice_maker
	src/main.cpp
	src/main_window.cpp
)

target_link_libraries(invoice_maker imgui_glfw)

