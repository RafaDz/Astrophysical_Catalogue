# PHYS 30762 Programming in C++
# Build file for the Astronomical Catalogue.
#
# Builds two programs that share the same catalogue code:
#   astro_catalogue      - the original terminal (menu) version
#   astro_catalogue_gui  - the graphical version (Dear ImGui + GLFW + OpenGL 3)
#
# Dear ImGui and GLFW are downloaded automatically the first time you configure,
# so the only requirements are CMake (>= 3.16), a C++17 compiler and OpenGL
# (already present on Windows/macOS; on Linux install the X11/GL dev packages).
#
#   cmake -S . -B build
#   cmake --build build
#
# The executables are placed in the project folder (next to 'Astro_Objects/').

cmake_minimum_required(VERSION 3.16)
project(AstrophysicalCatalogue LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ASTRO_BUILD_GUI "Build the Dear ImGui graphical interface" ON)

# Put executables in the project root so they find 'Astro_Objects/' and write to 'Exports/'.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
foreach(config ${CMAKE_CONFIGURATION_TYPES} Debug Release RelWithDebInfo MinSizeRel)
  string(TOUPPER ${config} config_upper)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config_upper} ${CMAKE_SOURCE_DIR})
endforeach()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# === Shared catalogue code (classes, loading, saving, exporting, menus) ===
add_library(catalogue_core STATIC
  celestial_objects.cpp
  galaxy.cpp
  star.cpp
  exoplanet.cpp
  objects_loader.cpp
  save_object.cpp
  add_object.cpp
  shared_functions.cpp
  export_galaxy_data.cpp
  export_star_data.cpp
  export_exoplanet_data.cpp
  sort_menu.cpp
  handle_sort_menu.cpp
  handle_export_menu.cpp
)
target_include_directories(catalogue_core PUBLIC ${CMAKE_SOURCE_DIR})

# === Terminal version ===
add_executable(astro_catalogue main_program.cpp)
target_link_libraries(astro_catalogue PRIVATE catalogue_core)

# === GUI version ===
if(ASTRO_BUILD_GUI)
  include(FetchContent)
  find_package(OpenGL REQUIRED)

  # Use a system-installed GLFW if there is one, otherwise download it.
  find_package(glfw3 3.3 QUIET)
  if(NOT glfw3_FOUND)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_WAYLAND  OFF CACHE BOOL "" FORCE) # X11 only (works under XWayland too)
    FetchContent_Declare(glfw
      URL https://github.com/glfw/glfw/archive/refs/tags/3.4.tar.gz)
    FetchContent_MakeAvailable(glfw)
  endif()

  FetchContent_Declare(imgui
    URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.9.tar.gz)
  FetchContent_MakeAvailable(imgui)

  add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
  )
  target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
  target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

  add_executable(astro_catalogue_gui
    gui/gui_main.cpp
    gui/catalogue_gui.cpp
  )
  target_link_libraries(astro_catalogue_gui PRIVATE catalogue_core imgui)
endif()
