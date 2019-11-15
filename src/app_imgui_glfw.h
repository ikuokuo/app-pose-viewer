#pragma once

#include "base/imgui_glfw_base.h"

#include "app_types.h"

class AppImGuiGlfw : public ImGuiGlfwBase {
 public:
  explicit AppImGuiGlfw(ImGuiBase *imgui_base = nullptr);
  ~AppImGuiGlfw() override;

  void Update(const AppDataPtr &data);

 protected:
  void OnInit() override;
  void OnDraw() override;
  void OnDestroy() override;

 private:
  GLuint vao_;
  GLuint vbo_;

  GLuint vertex_shader_;
  GLuint fragment_shader_;
  GLuint shader_program_;
};
