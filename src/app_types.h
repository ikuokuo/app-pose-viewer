#pragma once

#include <memory>

#include "glm/ext/vector_float4.hpp"

struct AppData {
  bool show_demo_window = false;
  glm::vec4 clear_color{0.f, 0.f, 0.f, 1.f};
};

using AppDataPtr = std::shared_ptr<AppData>;
