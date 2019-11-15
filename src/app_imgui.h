#pragma once

#include "base/imgui_base.h"

#include "app_types.h"

class AppImGui : public ImGuiBase {
 public:
  AppImGui();
  ~AppImGui() override;

  AppDataPtr GetData() const;

 protected:
  void OnDraw() override;

 private:
  AppDataPtr data_;
};
