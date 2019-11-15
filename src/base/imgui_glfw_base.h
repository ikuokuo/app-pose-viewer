#pragma once

#include "imgui.h"
#include "imgui_impl_opengl3.h"

// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually.
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#error IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include "glfw_base.h"

class ImGuiBase;

class ImGuiGlfwBase : public GlfwBase {
 public:
  explicit ImGuiGlfwBase(ImGuiBase *imgui_base = nullptr);
  ~ImGuiGlfwBase() override;

  int Run(
      const GlfwInitParams &params = GlfwInitParams{},
      GlfwRunCallback callback = nullptr) override;

 protected:
  void OnInit() override;

  bool default_;
  ImGuiBase *imgui_base_;
};
