#include <Arduino.h>

#include "log.h"
#include "wireless_handle_endpoint_rf24.h"

WirelessHandleEndpointRf24 wireless_handle_endpoint_rf24;

void OnButtonDown(void* user_parameter, uint8_t source_address, WirelessHandleEndpoint::ButtonType button_type) {
  LOG(INFO) << "source_address:" << source_address << ", button_type:" << button_type;
}

void OnButtonUp(void* user_parameter, uint8_t source_address, WirelessHandleEndpoint::ButtonType button_type) {
  LOG(INFO) << "source_address:" << source_address << ", button_type:" << button_type;
}

void OnButtonClick(void* user_parameter,
                   uint8_t source_address,
                   WirelessHandleEndpoint::ButtonType button_type,
                   uint8_t click_count) {
  LOG(INFO) << "source_address:" << source_address << ", button_type:" << button_type;
}

void OnButtonLongPressBegin(void* user_parameter, uint8_t source_address, WirelessHandleEndpoint::ButtonType button_type) {
  LOG(INFO) << "source_address:" << source_address << ", button_type:" << button_type;
}

void OnButtonDuringLongPress(void* user_parameter, uint8_t source_address, WirelessHandleEndpoint::ButtonType button_type) {
  LOG(INFO) << "source_address:" << source_address << ", button_type:" << button_type;
}

void OnButtonLongPressEnd(void* user_parameter, uint8_t source_address, WirelessHandleEndpoint::ButtonType button_type) {
  LOG(INFO) << "source_address:" << source_address << ", button_type:" << button_type;
}

void OnJoystickCoordinateUpdated(void* user_parameter, uint8_t source_address, int16_t x, int16_t y) {
  LOG(INFO) << "source_address:" << source_address << ", x: " << x << ", y: " << y;
}

void OnGravityAccelerationUpdated(void* user_parameter, uint8_t source_address, int16_t x, int16_t y, int16_t z) {
  LOG(INFO) << "source_address:" << source_address << ", x: " << x << ", y: " << y << ", z:" << z;
}

void OnMessage(const WirelessHandleEndpoint::Message& message) {
  switch (message.message_type) {
    case WirelessHandleEndpoint::kButtonDown: {
      OnButtonDown(message.user_parameter, GetSourceAddress(message), GetButtonType(message));
      break;
    }
    case WirelessHandleEndpoint::kButtonUp: {
      OnButtonUp(message.user_parameter, GetSourceAddress(message), GetButtonType(message));
      break;
    }
    case WirelessHandleEndpoint::kButtonClick: {
      OnButtonClick(message.user_parameter, GetSourceAddress(message), GetButtonType(message), GetButtonClickCount(message));
      break;
    }
    case WirelessHandleEndpoint::kButtonLongPressBegin: {
      OnButtonLongPressBegin(message.user_parameter, GetSourceAddress(message), GetButtonType(message));
      break;
    }
    case WirelessHandleEndpoint::kButtonDuringLongPress: {
      OnButtonDuringLongPress(message.user_parameter, GetSourceAddress(message), GetButtonType(message));
      break;
    }
    case WirelessHandleEndpoint::kButtonLongPressEnd: {
      OnButtonLongPressEnd(message.user_parameter, GetSourceAddress(message), GetButtonType(message));
      break;
    }
    case WirelessHandleEndpoint::kGravityAccelerationChange: {
      OnGravityAccelerationUpdated(message.user_parameter,
                                   GetSourceAddress(message),
                                   GetGravityAccelerationX(message),
                                   GetGravityAccelerationY(message),
                                   GetGravityAccelerationZ(message));
      break;
    }
    case WirelessHandleEndpoint::kJoystickCoordinateChange: {
      OnJoystickCoordinateUpdated(
          message.user_parameter, GetSourceAddress(message), GetJoystickCoordinateX(message), GetJoystickCoordinateY(message));
      break;
    }
    default: {
      break;
    }
  }
}

void setup() {
  Serial.begin(115200);
  wireless_handle_endpoint_rf24.SetMessageHandler(OnMessage, nullptr);
  wireless_handle_endpoint_rf24.SetupRf24(115, 5, 0x0011000011LL);
  LOG(INFO);
}

void loop() {
  wireless_handle_endpoint_rf24.Tick();
  // Serial.write(0x11);
  // delay(10);
}