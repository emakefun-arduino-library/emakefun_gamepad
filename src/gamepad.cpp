#include "gamepad.h"

#include <Arduino.h>

namespace emakefun {

namespace {
enum Pin : uint8_t {
  kPinButtonJoystick = A0,
  kPinButtonL = A1,
  kPinButtonR = 6,
  kPinButtonSelect = 10,
  kPinButtonMode = A3,
  kPinButtonA = 3,
  kPinButtonB = 9,
  kPinButtonC = 5,
  kPinButtonD = 4,
  kPinRockerCoordinateX = A6,
  kPinRockerCoordinateY = A7,
};
}  // namespace

Gamepad::Gamepad()
    : buttons_({emakefun::Button(kPinButtonJoystick),
                emakefun::Button(kPinButtonL),
                emakefun::Button(kPinButtonR),
                emakefun::Button(kPinButtonSelect),
                emakefun::Button(kPinButtonMode),
                emakefun::Button(kPinButtonA),
                emakefun::Button(kPinButtonB),
                emakefun::Button(kPinButtonC),
                emakefun::Button(kPinButtonD)}) {
}

void Gamepad::Initialize() {
  mpu6050_.Setup();
  pinMode(kPinRockerCoordinateX, INPUT);
  pinMode(kPinRockerCoordinateY, INPUT);
  for (const auto& button : buttons_) {
    button.Initialize();
  }
}

void Gamepad::Tick() {
  if (model_ != nullptr) {
    model_->Tick();
  }
  uint16_t button_state = 0;
  for (uint8_t i = 0; i < GamepadModel::kButtonMax; i++) {
    buttons_[i].Tick();
    bitWrite(button_state, i, buttons_[i].IsActive());
  }

  if (model_ != nullptr) {
    model_->OnButtonState(button_state);
    model_->OnJoystickCoordinate({analogRead(kPinRockerCoordinateX) >> 2, analogRead(kPinRockerCoordinateY) >> 2});
    if (mpu6050_.UpdateMotionInfo()) {
      const auto acceleration = mpu6050_.GetAcceleration();
      model_->OnGravityAcceleration({acceleration.x, acceleration.y, acceleration.z});
    }
  }
}

void Gamepad::AttachModel(const GamepadModel* model) {
  model_ = model;
}
}  // namespace emakefun