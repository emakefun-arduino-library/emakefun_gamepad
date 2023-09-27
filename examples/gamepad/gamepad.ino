#include <Arduino.h>

#include "gamepad.h"
#include "log.h"

emakefun::Gamepad g_gamepad;
emakefun::GamepadModel g_gamepad_model;

void setup() {
  Serial.begin(115200);
  LOG(INFO);
  g_gamepad.Initialize();
  g_gamepad.AttachModel(&g_gamepad_model);
}

void loop() {
  g_gamepad.Tick();
  if (g_gamepad_model.NewButtonState()) {
    LOG(INFO) << "new button state";
  }

  for (uint8_t button_type = emakefun::GamepadModel::kButtonJoystick; button_type < emakefun::GamepadModel::kButtonMax;
       button_type++) {
    if (g_gamepad_model.NewButtonState(button_type)) {
      LOG(INFO) << "new button state: " << button_type;
    }

    if (g_gamepad_model.ButtonPressed(button_type)) {
      LOG(INFO) << "button pressed: " << button_type;
    }

    if (g_gamepad_model.ButtonReleased(button_type)) {
      LOG(INFO) << "button released: " << button_type;
    }

    if (g_gamepad_model.GetButtonState(button_type)) {
      LOG(INFO) << "button is being held: " << button_type;
    }
  }

  if (g_gamepad_model.NewJoystickCoordinate()) {
    // g_joystick_coordinate = g_gamepad_model.GetJoystickCoordinate();
    LOG(INFO) << "joystick coordinate: " << g_gamepad_model.GetJoystickCoordinate().x << ", "
              << g_gamepad_model.GetJoystickCoordinate().y;
  }
#if 1
  if (g_gamepad_model.NewGravityAcceleration()) {
    LOG(INFO) << "gravity acceleration: " << g_gamepad_model.GetGravityAcceleration().x << ", "
              << g_gamepad_model.GetGravityAcceleration().y << ", " << g_gamepad_model.GetGravityAcceleration().z;
  }
#endif
}