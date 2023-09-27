#pragma once

#include "gamepad_model.h"
#include "mpu6050.h"
#include "utility/List.h"
#include "utility/button.h"

namespace emakefun {
class Gamepad {
 public:
  Gamepad();
  virtual ~Gamepad() = default;
  void Initialize();
  void Tick();
  void AttachModel(const GamepadModel* model);

 private:
  Mpu6050 mpu6050_;
  const emakefun::Button buttons_[GamepadModel::kButtonMax];
  GamepadModel* model_ = nullptr;
};
}  // namespace emakefun