#include <Arduino.h>

#include "log.h"
#include "wireless_handle_controller.h"
#include "wireless_handle_controller_rf24.h"

WirelessHandleControllerRf24 wireless_handle_controller;

void OnButtonDown(void* user_parameter, WirelessHandle::ButtonType button_type) {
  LOG(INFO) << "button_type:" << button_type;
}

void OnButtonUp(void* user_parameter, WirelessHandle::ButtonType button_type) {
  LOG(INFO) << "button_type:" << button_type;
}

void OnButtonClick(void* user_parameter, WirelessHandle::ButtonType button_type, uint8_t click_count) {
  LOG(INFO) << "button_type:" << button_type;
}

void OnButtonLongPressBegin(void* user_parameter, WirelessHandle::ButtonType button_type) {
  LOG(INFO) << "button_type:" << button_type;
}

void OnButtonDuringLongPress(void* user_parameter, WirelessHandle::ButtonType button_type) {
  LOG(INFO) << "button_type:" << button_type;
}

void OnButtonLongPressEnd(void* user_parameter, WirelessHandle::ButtonType button_type) {
  LOG(INFO) << "button_type:" << button_type;
}

void OnJoystickCoordinateUpdated(void* user_parameter, int16_t x, int16_t y) {
  LOG(INFO) << "x: " << x << ", y: " << y;
}

void OnGravityAccelerationUpdated(void* user_parameter, int16_t x, int16_t y, int16_t z) {
  LOG(INFO) << "x: " << x << ", y: " << y << ", z:" << z;
}

void OnMessage(const WirelessHandle::Message& message) {
  switch (message.message_type) {
    case WirelessHandle::kButtonDown: {
      OnButtonDown(message.user_parameter, GetButtonType(message));
      break;
    }
    case WirelessHandle::kButtonUp: {
      OnButtonUp(message.user_parameter, GetButtonType(message));
      break;
    }
    case WirelessHandle::kButtonClick: {
      OnButtonClick(message.user_parameter, GetButtonType(message), GetButtonClickCount(message));
      break;
    }
    case WirelessHandle::kButtonLongPressBegin: {
      OnButtonLongPressBegin(message.user_parameter, GetButtonType(message));
      break;
    }
    case WirelessHandle::kButtonDuringLongPress: {
      OnButtonDuringLongPress(message.user_parameter, GetButtonType(message));
      break;
    }
    case WirelessHandle::kButtonLongPressEnd: {
      OnButtonLongPressEnd(message.user_parameter, GetButtonType(message));
      break;
    }
    case WirelessHandle::kGravityAccelerationChange: {
      OnGravityAccelerationUpdated(message.user_parameter,
                                   GetGravityAccelerationX(message),
                                   GetGravityAccelerationY(message),
                                   GetGravityAccelerationZ(message));
      break;
    }
    case WirelessHandle::kJoystickCoordinateChange: {
      OnJoystickCoordinateUpdated(message.user_parameter, GetJoystickCoordinateX(message), GetJoystickCoordinateY(message));
      break;
    }
    default: {
      break;
    }
  }
}

void setup() {
  Serial.begin(115200);
  wireless_handle_controller.Setup();
  wireless_handle_controller.SetMessageHandler(OnMessage, nullptr);
  wireless_handle_controller.SetSourceAddress(2);
  wireless_handle_controller.SetupRf24(115, 5, 0x0011000011LL);
  LOG(INFO);
}

void loop() {
  wireless_handle_controller.Tick();
  // delay(10);
}