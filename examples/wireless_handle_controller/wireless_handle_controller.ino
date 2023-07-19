#include <Arduino.h>

#include "log.h"
#include "wireless_handle_controller.h"

WirelessHandleController wireless_handle_controller;

void setup() {
  Serial.begin(115200);
  wireless_handle_controller.Setup();
  LOG(INFO);
}

void loop() {
  wireless_handle_controller.Tick();
  if (wireless_handle_controller.NewButtonState()) {
    LOG(INFO) << "new button state";
  }

  for (uint8_t button_type = WirelessHandle::kButtonJoystick; button_type < WirelessHandle::kButtonMax; button_type++) {
    if (wireless_handle_controller.NewButtonState(button_type)) {
      LOG(INFO) << "new button state: " << button_type;
    }

    if (wireless_handle_controller.ButtonPressed(button_type)) {
      LOG(INFO) << "button pressed: " << button_type;
    }

    if (wireless_handle_controller.ButtonReleased(button_type)) {
      LOG(INFO) << "button released: " << button_type;
    }

    if (wireless_handle_controller.GetButtonState(button_type)) {
      LOG(INFO) << "button is being held: " << button_type;
    }
  }

  if (wireless_handle_controller.NewJoystickCoordinate()) {
    LOG(INFO) << "joystick coordinate: " << wireless_handle_controller.GetJoystickCoordinate().x << ", "
              << wireless_handle_controller.GetJoystickCoordinate().y;
  }

  if (wireless_handle_controller.NewGravityAcceleration()) {
    LOG(INFO) << "gravity acceleration: " << wireless_handle_controller.GetGravityAcceleration().x << ", "
              << wireless_handle_controller.GetGravityAcceleration().y << ", "
              << wireless_handle_controller.GetGravityAcceleration().z;
  }
}