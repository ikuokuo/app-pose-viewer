#include "imgui_glfw_base.h"

#include <cassert>
#include <iostream>
#include <memory>

#include "imgui_base.h"

namespace {

class GlfwBaseCallbackImpl : public GlfwBaseCallback {
 public:
  explicit GlfwBaseCallbackImpl(ImGuiBase *imgui_base)
    : imgui_base_(imgui_base) {
    assert(imgui_base_);
  }

  void OnGlfwInit(GlfwBase *glfw_base) override {
    imgui_base_->Init(glfw_base->GetWindow(),
        glfw_base->GetGLSLVersion().c_str());
  }

  void OnGlfwDraw(GlfwBase *) override {
    imgui_base_->Draw();
  }

  void OnGlfwDestory(GlfwBase *) override {
    imgui_base_->Destroy();
  }

 private:
  ImGuiBase *imgui_base_;
};

}  // namespace

ImGuiGlfwBase::ImGuiGlfwBase(ImGuiBase *imgui_base)
  : default_(false),
    imgui_base_(imgui_base) {
  if (!imgui_base_) {
    imgui_base_ = new ImGuiBase;
    default_ = true;
  }
}

ImGuiGlfwBase::~ImGuiGlfwBase() {
  if (default_) {
    delete imgui_base_;
  }
}

int ImGuiGlfwBase::Run(
    const GlfwInitParams &params,
    GlfwRunCallback callback) {
  if (imgui_base_) {
    SetCallback(std::make_shared<GlfwBaseCallbackImpl>(imgui_base_));
  }
  return GlfwBase::Run(params, callback);
}

void ImGuiGlfwBase::OnInit() {
  // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
  bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
  bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
  bool err = gladLoadGL() == 0;
#else
  bool err = false;  // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
  if (err) {
    std::cerr << "Failed to initialize OpenGL loader!" << std::endl;
    assert(false);
  }

  std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
  std::cout << "OpenGL version supported " << glGetString(GL_VERSION) << std::endl;
}
