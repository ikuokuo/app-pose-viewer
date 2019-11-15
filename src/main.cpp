#include "app_imgui.h"
#include "app_imgui_glfw.h"

int main(int argc, char const *argv[]) {
  (void)argc;
  (void)argv;
  AppImGui imgui;
  AppImGuiGlfw glfw(&imgui);
  return glfw.Run({1280, 720, "Pose Viewer"}, [&](GlfwBase *) {
    glfw.Update(imgui.GetData());
  });
}
