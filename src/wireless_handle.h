#pragma once

class WirelessHandle {
 public:
  enum ButtonType : uint8_t {
    kButtonJoystick = 0x00,
    kButtonLeft = 0x01,
    kButtonRight = 0x02,
    kButtonSelect = 0x03,
    kButtonMode = 0x04,
    kButtonWhite = 0x05,
    kButtonYellow = 0x06,
    kButtonBlue = 0x07,
    kButtonRed = 0x08,
    kButtonMax,
  };

  enum : uint8_t {
    kDeviceType = 0x01,
  };

  enum : uint8_t {
    kDefaultDeviceAddress = kDeviceType,
  };

  enum MessageType : uint8_t {
    kButtonDown = 0x00,
    kButtonUp = 0x01,
    kButtonClick = 0x02,
    kButtonLongPressBegin = 0x03,
    kButtonDuringLongPress = 0x04,
    kButtonLongPressEnd = 0x05,
    kGravityAccelerationChange = 0x10,
    kJoystickCoordinateChange = 0x20,
  };
#pragma pack(1)
  struct Message {
    MessageType message_type;
    void* user_parameter;
  };

  struct ButtonEventMessage : public Message {
    ButtonEventMessage(const Message& message, const ButtonType button_type) : Message(message), button_type(button_type) {
    }

    ButtonType button_type;
  };

  struct ButtonClickMessage : public ButtonEventMessage {
    ButtonClickMessage(const ButtonEventMessage& message_button_event, const uint8_t click_count)
        : ButtonEventMessage(message_button_event), click_count(click_count) {
    }

    uint8_t click_count;
  };

  struct GravityAccelerationChangeMessage : public Message {
    GravityAccelerationChangeMessage(const Message& message, const int16_t x, const int16_t y, const int16_t z)
        : Message(message), x(x), y(y), z(z) {
    }
    int16_t x;
    int16_t y;
    int16_t z;
  };

  struct JoystickCoordinateChangeMessage : public Message {
    JoystickCoordinateChangeMessage(const Message& message, const int16_t x, const int16_t y) : Message(message), x(x), y(y) {
    }
    int16_t x;
    int16_t y;
  };
#pragma pack()

  using OnMessage = void(const Message&);

  WirelessHandle() = default;

  virtual ~WirelessHandle() = default;

  virtual void SetMessageHandler(const OnMessage&, void* user_parameter) = 0;

 private:
  WirelessHandle(const WirelessHandle&) = delete;
  WirelessHandle& operator=(const WirelessHandle&) = delete;
};

namespace {
inline WirelessHandle::ButtonType GetButtonType(const WirelessHandle::Message& message) {
  return reinterpret_cast<const WirelessHandle::ButtonEventMessage&>(message).button_type;
}

inline uint8_t GetButtonClickCount(const WirelessHandle::Message& message) {
  return reinterpret_cast<const WirelessHandle::ButtonClickMessage&>(message).click_count;
}

inline int16_t GetJoystickCoordinateX(const WirelessHandle::Message& message) {
  return reinterpret_cast<const WirelessHandle::JoystickCoordinateChangeMessage&>(message).x;
}

inline int16_t GetJoystickCoordinateY(const WirelessHandle::Message& message) {
  return reinterpret_cast<const WirelessHandle::JoystickCoordinateChangeMessage&>(message).y;
}

inline int16_t GetGravityAccelerationX(const WirelessHandle::Message& message) {
  return reinterpret_cast<const WirelessHandle::GravityAccelerationChangeMessage&>(message).x;
}

inline int16_t GetGravityAccelerationY(const WirelessHandle::Message& message) {
  return reinterpret_cast<const WirelessHandle::GravityAccelerationChangeMessage&>(message).y;
}

inline int16_t GetGravityAccelerationZ(const WirelessHandle::Message& message) {
  return reinterpret_cast<const WirelessHandle::GravityAccelerationChangeMessage&>(message).z;
}
}  // namespace