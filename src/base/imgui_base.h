#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class ImGuiBase {
 public:
  ImGuiBase();
  virtual ~ImGuiBase();

  bool Init(GLFWwindow *window, const char *glsl_version = NULL);
  void Draw();
  void Destroy();

 protected:
  virtual void OnInit();
  virtual void OnDraw();
  virtual void OnDestroy();
};
