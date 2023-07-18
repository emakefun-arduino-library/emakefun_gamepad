#pragma once

#include "wireless_handle.h"

class WirelessHandleEndpoint : public WirelessHandle {
 public:
#pragma pack(1)
  struct EndpointMessage {
    uint8_t source_address;
  };

  struct EndpointButtonEventMessage : public EndpointMessage, ButtonEventMessage {
    EndpointButtonEventMessage(const EndpointMessage& endpoint_message, const ButtonEventMessage& button_event_message)
        : EndpointMessage(endpoint_message), ButtonEventMessage(button_event_message) {
    }
  };

  struct EndpointButtonClickMessage : public EndpointMessage, ButtonClickMessage {
    EndpointButtonClickMessage(const EndpointMessage& endpoint_message, const ButtonClickMessage& button_click_message)
        : EndpointMessage(endpoint_message), ButtonClickMessage(button_click_message) {
    }
  };

  struct EndpointGravityAccelerationChangeMessage : public EndpointMessage, GravityAccelerationChangeMessage {
    EndpointGravityAccelerationChangeMessage(const EndpointMessage& endpoint_message,
                                             const GravityAccelerationChangeMessage& gravity_acceleration_change_message)
        : EndpointMessage(endpoint_message), GravityAccelerationChangeMessage(gravity_acceleration_change_message) {
    }
  };

  struct EndpointJoystickCoordinateChangeMessage : public EndpointMessage, JoystickCoordinateChangeMessage {
    EndpointJoystickCoordinateChangeMessage(const EndpointMessage& endpoint_message,
                                            const JoystickCoordinateChangeMessage& joystick_coordinate_change_message)
        : EndpointMessage(endpoint_message), JoystickCoordinateChangeMessage(joystick_coordinate_change_message) {
    }
  };
#pragma pack()

  virtual ~WirelessHandleEndpoint() = default;
};

namespace {

uint8_t GetSourceAddress(const WirelessHandle::Message& message) {
  return reinterpret_cast<const WirelessHandleEndpoint::EndpointMessage*>(reinterpret_cast<uintptr_t>(&message) -
                                                                          sizeof(WirelessHandleEndpoint::EndpointMessage))
      ->source_address;
}
}  // namespace