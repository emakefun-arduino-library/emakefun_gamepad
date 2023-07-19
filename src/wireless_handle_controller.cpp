#include "wireless_handle_controller.h"

#include <Arduino.h>

#include "log.h"

namespace {
enum Pin : uint8_t {
  kPinButtonJoystick = A0,
  // kPinButtonL = 7,
  kPinButtonL = 6,
  kPinButtonR = 6,
  // kPinButtonSelect = 8,
  kPinButtonSelect = 6,
  kPinButtonMode = A3,
  kPinButtonWhite = 3,
  kPinButtonYellow = 4,
  kPinButtonBlue = 2,
  kPinButtonRed = 5,
  kPinRockerCoordinateX = A6,
  kPinRockerCoordinateY = A7,
};

constexpr uint8_t kMinUpdateInterval = 500;
constexpr uint8_t kMinJoystickCoordinateDiff = 10;
constexpr uint8_t kMinGravityAccelerationDiff = 10;
}  // namespace

WirelessHandleController::WirelessHandleController()
    : buttons_({AgileButton(kPinButtonJoystick),
                AgileButton(kPinButtonL),
                AgileButton(kPinButtonR),
                AgileButton(kPinButtonSelect),
                AgileButton(kPinButtonMode),
                AgileButton(kPinButtonWhite),
                AgileButton(kPinButtonYellow),
                AgileButton(kPinButtonBlue),
                AgileButton(kPinButtonRed)}) {
  for (uint8_t button_type = kButtonJoystick; button_type != kButtonMax; button_type++) {
    buttons_[button_type].SetEventHandler(this, reinterpret_cast<void*>(button_type));
  }
}

WirelessHandleController::~WirelessHandleController() = default;

bool WirelessHandleController::Setup() {
  auto ret = mpu6050_.Setup();
  if (!ret) {
    return false;
  }

  pinMode(kPinRockerCoordinateX, INPUT);
  pinMode(kPinRockerCoordinateY, INPUT);
  return true;
}

void WirelessHandleController::Tick() {
  WirelessHandle::Tick();
  const auto now = millis();
  for (const auto& button : buttons_) {
    button.Tick();
  }

  if (button_state_ != last_button_state_ || last_update_button_state_time_ + kMinUpdateInterval < now) {
    last_update_button_state_time_ = now;
    OnUpdateButtonState();
  }

  JoystickCoordinate joystick_coordinate{
      .x = analogRead(kPinRockerCoordinateX),
      .y = analogRead(kPinRockerCoordinateY),
  };

  if (abs(joystick_coordinate.x - joystick_coordinate_.x) > kMinJoystickCoordinateDiff ||
      abs(joystick_coordinate.y - joystick_coordinate_.y) > kMinJoystickCoordinateDiff ||
      last_update_joystick_coordinate_time_ + kMinUpdateInterval < now) {
    last_update_joystick_coordinate_time_ = now;
    joystick_coordinate_ = joystick_coordinate;
    OnUpdateJoystickCoordinate();
  }

  if (mpu6050_.UpdateMotionInfo()) {
    const auto acceleration = mpu6050_.GetAcceleration();
    if (abs(gravity_acceleration_.x - acceleration.x) > kMinGravityAccelerationDiff ||
        abs(gravity_acceleration_.y - acceleration.y) > kMinGravityAccelerationDiff ||
        abs(gravity_acceleration_.z - acceleration.z) > kMinGravityAccelerationDiff ||
        last_update_gravity_acceleration_time_ + kMinUpdateInterval < now) {
      last_update_gravity_acceleration_time_ = now;
      gravity_acceleration_.x = acceleration.x;
      gravity_acceleration_.y = acceleration.y;
      gravity_acceleration_.z = acceleration.z;
      OnUpdateGravityAcceleration();
    }
  }
}

void WirelessHandleController::OnButtonEvent(const AgileButton::Event& event) {
  const uint8_t button_type = reinterpret_cast<uint8_t>(event.user_parameter);
  switch (event.event_type) {
    case AgileButton::kButtonDown: {
      // SendMessage(ButtonEventMessage{{kButtonDown, user_parameter_}, button_type});
      // LOG(INFO) << "ButtonDown: " << button_type;
      button_state_ |= static_cast<uint16_t>(1) << button_type;
      break;
    }
    case AgileButton::kButtonUp: {
      // SendMessage(ButtonEventMessage{{kButtonUp, user_parameter_}, button_type});
      button_state_ &= ~(static_cast<uint16_t>(1) << button_type);
      // LOG(INFO) << "ButtonUp: " << button_type;
      break;
    }
    default: {
      break;
    }
  }
}
