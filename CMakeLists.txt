cmake_minimum_required(VERSION 3.13)

project(invoice_maker LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)
add_definitions(-DINVOICE_MAKER_DEBUG)

find_package(SQLite3 REQUIRED)

add_subdirectory(vendor)

add_executable(invoice_maker
	src/main.cpp
	src/main_window.cpp
	src/database.cpp
	src/data/migrations.cpp
)

target_link_libraries(invoice_maker PRIVATE imgui_glfw)
target_link_libraries(invoice_maker PRIVATE SQLite::SQLite3)

