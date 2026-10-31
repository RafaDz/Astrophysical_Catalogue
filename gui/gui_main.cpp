// PHYS 30762 Programming in C++
/* Entry point for the graphical version of the Astronomical Catalogue.
 * Creates a window with GLFW + OpenGL 3, sets up Dear ImGui and runs the main loop.
 * All catalogue logic lives in CatalogueApp (catalogue_gui.cpp).
 */

#include "catalogue_gui.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>

#include<cstdio> // for std::fprintf
#include<filesystem> // for std::filesystem
#include<string> // for std::string
#include<system_error> // for std::error_code

namespace fs = std::filesystem;

static void glfw_error_callback(int error, const char* description)
{
  std::fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

// The data folder is relative to the working directory. If the program is started
// from somewhere else (e.g. double-clicked), switch to the folder of the executable.
static void find_data_folder(const char* argv0)
{
  if(fs::exists("Astro_Objects")) return;
  std::error_code error;
  fs::path exe_folder = fs::absolute(fs::path(argv0), error).parent_path();
  if(!error && fs::exists(exe_folder / "Astro_Objects"))
    fs::current_path(exe_folder, error);
}

static void apply_style(float scale)
{
  ImGui::StyleColorsDark();
  ImGuiStyle& style = ImGui::GetStyle();
  style.WindowRounding = 0.0f;
  style.ChildRounding = 4.0f;
  style.FrameRounding = 4.0f;
  style.PopupRounding = 4.0f;
  style.GrabRounding = 4.0f;
  style.TabRounding = 4.0f;
  style.FramePadding = ImVec2(8.0f, 4.0f);
  style.ItemSpacing = ImVec2(8.0f, 6.0f);
  style.CellPadding = ImVec2(6.0f, 3.0f);

  // A deep-space blue theme
  ImVec4* colours = style.Colors;
  colours[ImGuiCol_WindowBg]        = ImVec4(0.07f, 0.08f, 0.12f, 1.00f);
  colours[ImGuiCol_ChildBg]         = ImVec4(0.09f, 0.10f, 0.15f, 1.00f);
  colours[ImGuiCol_PopupBg]         = ImVec4(0.10f, 0.11f, 0.17f, 0.98f);
  colours[ImGuiCol_MenuBarBg]       = ImVec4(0.10f, 0.11f, 0.17f, 1.00f);
  colours[ImGuiCol_Header]          = ImVec4(0.20f, 0.32f, 0.55f, 0.70f);
  colours[ImGuiCol_HeaderHovered]   = ImVec4(0.26f, 0.40f, 0.68f, 0.80f);
  colours[ImGuiCol_HeaderActive]    = ImVec4(0.30f, 0.46f, 0.78f, 1.00f);
  colours[ImGuiCol_TableHeaderBg]   = ImVec4(0.13f, 0.15f, 0.23f, 1.00f);
  colours[ImGuiCol_TableRowBgAlt]   = ImVec4(1.00f, 1.00f, 1.00f, 0.03f);
  colours[ImGuiCol_Tab]             = ImVec4(0.13f, 0.16f, 0.26f, 1.00f);
  colours[ImGuiCol_TabHovered]      = ImVec4(0.26f, 0.40f, 0.68f, 1.00f);
  colours[ImGuiCol_TabSelected]     = ImVec4(0.20f, 0.32f, 0.55f, 1.00f);

  style.ScaleAllSizes(scale);
}

int main(int, char** argv)
{
  find_data_folder(argv[0]);

  glfwSetErrorCallback(glfw_error_callback);
  if(!glfwInit())
  {
    std::fprintf(stderr, "Could not initialise GLFW (is a display available?).\n");
    return 1;
  }

  // OpenGL 3.x context (3.2 core on macOS)
#if defined(__APPLE__)
  const char* glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

  // Scale the UI on high-DPI monitors
  float scale = 1.0f;
  if(GLFWmonitor* monitor = glfwGetPrimaryMonitor())
  {
    float x_scale = 1.0f, y_scale = 1.0f;
    glfwGetMonitorContentScale(monitor, &x_scale, &y_scale);
    if(x_scale > 0.5f) scale = x_scale;
  }

  GLFWwindow* window = glfwCreateWindow(static_cast<int>(1280 * scale), static_cast<int>(800 * scale),
                                        "Astronomical Catalogue", nullptr, nullptr);
  if(!window)
  {
    std::fprintf(stderr, "Could not create a window with OpenGL 3.\n");
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // vsync

  // === Dear ImGui setup ===
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.IniFilename = "astro_catalogue_gui.ini"; // Remembers column widths/order between runs

  apply_style(scale);
  ImFontConfig font_config;
  font_config.SizePixels = 15.0f * scale;
  io.Fonts->AddFontDefault(&font_config);

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  CatalogueApp app;
  std::string current_title;

  // === Main loop ===
  while(!app.should_quit())
  {
    glfwPollEvents();

    // Closing the window asks for confirmation if there are unsaved changes
    if(glfwWindowShouldClose(window))
    {
      glfwSetWindowShouldClose(window, GLFW_FALSE);
      app.request_quit();
    }

    // Avoid using CPU while minimised
    if(glfwGetWindowAttrib(window, GLFW_ICONIFIED))
    {
      glfwWaitEventsTimeout(0.1);
      continue;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    app.draw();

    std::string title = app.window_title();
    if(title != current_title)
    {
      glfwSetWindowTitle(window, title.c_str());
      current_title = title;
    }

    ImGui::Render();
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.05f, 0.06f, 0.09f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  // === Cleanup ===
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
