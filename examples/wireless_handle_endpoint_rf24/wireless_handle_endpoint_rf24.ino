#include <Arduino.h>

#include "log.h"
#include "wireless_handle_endpoint_rf24.h"

WirelessHandleEndpointRf24 wireless_handle_endpoint_rf24;

void setup() {
  Serial.begin(115200);
  wireless_handle_endpoint_rf24.SetupRf24(115, 5, 0x0011000011LL);
  LOG(INFO);
}

void loop() {
  wireless_handle_endpoint_rf24.Tick();
  if (wireless_handle_endpoint_rf24.NewButtonState()) {
    LOG(INFO) << "new button state";
  }

  for (uint8_t button_type = WirelessHandle::kButtonJoystick; button_type < WirelessHandle::kButtonMax; button_type++) {
    if (wireless_handle_endpoint_rf24.NewButtonState(button_type)) {
      LOG(INFO) << "new button state: " << button_type;
    }

    if (wireless_handle_endpoint_rf24.ButtonPressed(button_type)) {
      LOG(INFO) << "button pressed: " << button_type;
    }

    if (wireless_handle_endpoint_rf24.ButtonReleased(button_type)) {
      LOG(INFO) << "button released: " << button_type;
    }

    if (wireless_handle_endpoint_rf24.GetButtonState(button_type)) {
      LOG(INFO) << "button is being held: " << button_type;
    }
  }

  if (wireless_handle_endpoint_rf24.NewJoystickCoordinate()) {
    LOG(INFO) << "joystick coordinate: " << wireless_handle_endpoint_rf24.GetJoystickCoordinate().x << ", "
              << wireless_handle_endpoint_rf24.GetJoystickCoordinate().y;
  }

  if (wireless_handle_endpoint_rf24.NewGravityAcceleration()) {
    LOG(INFO) << "gravity acceleration: " << wireless_handle_endpoint_rf24.GetGravityAcceleration().x << ", "
              << wireless_handle_endpoint_rf24.GetGravityAcceleration().y << ", "
              << wireless_handle_endpoint_rf24.GetGravityAcceleration().z;
  }
}