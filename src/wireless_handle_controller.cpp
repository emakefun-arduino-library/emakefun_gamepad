#include "wireless_handle_controller.h"

#include <Arduino.h>

#include "log.h"

namespace {
enum Pin : uint8_t {
  kPinButtonJoystick = A0,
  // kPinButtonLeft = 7,
  kPinButtonLeft = 6,
  kPinButtonRight = 6,
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
}

WirelessHandleController::WirelessHandleController()
    : buttons_({AgileButton(kPinButtonJoystick),
                AgileButton(kPinButtonLeft),
                AgileButton(kPinButtonRight),
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

WirelessHandleController::~WirelessHandleController() {
}

bool WirelessHandleController::Setup() {
  auto ret = mpu6050_.Setup();
  if (!ret) {
    return false;
  }

  pinMode(kPinRockerCoordinateX, INPUT);
  pinMode(kPinRockerCoordinateY, INPUT);
  return true;
}

void WirelessHandleController::SetMessageHandler(const OnMessage& on_message, void* user_parameter) {
  on_message_ = on_message;
  user_parameter_ = user_parameter;
}

void WirelessHandleController::SendMessage(const Message& message) {
  if (on_message_ != nullptr) {
    on_message_(message);
  }
}

void WirelessHandleController::Tick() {
  const auto now = millis();
  for (const auto& button : buttons_) {
    button.Tick();
  }

  if (mpu6050_.UpdateMotionInfo()) {
    const auto acceleration = mpu6050_.GetAcceleration();
    constexpr int16_t acceleration_diff_threshold = 10;
    if (abs(acceleration.x - acceleration_.x) > acceleration_diff_threshold ||
        abs(acceleration.y - acceleration_.y) > acceleration_diff_threshold ||
        abs(acceleration.z - acceleration_.z) > acceleration_diff_threshold) {
      acceleration_ = acceleration;
      last_update_acceleration_time_ = now;
      // LOG(INFO) << "acceleration, x: " << acceleration.x << ", y: " << acceleration.y << ", z: " << acceleration.z;
      SendMessage(GravityAccelerationChangeMessage{
          {kGravityAccelerationChange, user_parameter_}, acceleration.x, acceleration.y, acceleration.z});
    }
  }

  const int16_t joystick_coordinate_x = analogRead(kPinRockerCoordinateX);
  const int16_t joystick_coordinate_y = analogRead(kPinRockerCoordinateY);
  constexpr int16_t coordinate_diff_threshold = 10;
  if (abs(joystick_coordinate_x - joystick_coordinate_x_) > coordinate_diff_threshold ||
      abs(joystick_coordinate_y - joystick_coordinate_y_) > coordinate_diff_threshold) {
    joystick_coordinate_x_ = joystick_coordinate_x;
    joystick_coordinate_y_ = joystick_coordinate_y;
    last_update_joystick_coordinate_time_ = now;
    // LOG(INFO) << "joystick coordinate x: " << joystick_coordinate_x << ", y: " << joystick_coordinate_y;
    SendMessage(JoystickCoordinateChangeMessage{
        {kJoystickCoordinateChange, user_parameter_}, joystick_coordinate_x, joystick_coordinate_y});
  }
}

void WirelessHandleController::OnButtonEvent(const AgileButton::Event& event) {
  const uint8_t button_type = reinterpret_cast<uint8_t>(event.user_parameter);

  switch (event.event_type) {
    case AgileButton::kButtonDown: {
      SendMessage(ButtonEventMessage{{kButtonDown, user_parameter_}, button_type});
      // LOG(INFO) << "ButtonDown: " << button_type;
      break;
    }
    case AgileButton::kButtonUp: {
      SendMessage(ButtonEventMessage{{kButtonUp, user_parameter_}, button_type});
      // LOG(INFO) << "ButtonUp: " << button_type;
      break;
    }
    case AgileButton::kButtonClick: {
      SendMessage(ButtonClickMessage{{{kButtonClick, user_parameter_}, button_type}, GetButtonClickCount(event)});
      // LOG(INFO) << "ButtonClick: " << button_type << ", click count: " << click_count;
      break;
    }
    case AgileButton::kButtonLongPressBegin: {
      SendMessage(ButtonEventMessage{{kButtonLongPressBegin, user_parameter_}, button_type});
      // LOG(INFO) << "ButtonLongPressBegin: " << button_type;
      break;
    }
    case AgileButton::kButtonDuringLongPress: {
      SendMessage(ButtonEventMessage{{kButtonDuringLongPress, user_parameter_}, button_type});
      // LOG(INFO) << "kButtonDuringLongPress: " << button_type;
      break;
    }
    case AgileButton::kButtonLongPressEnd: {
      SendMessage(ButtonEventMessage{{kButtonLongPressEnd, user_parameter_}, button_type});
      // LOG(INFO) << "kButtonDuringLongPress: " << button_type;
      break;
    }
    default: {
      break;
    }
  }
}
