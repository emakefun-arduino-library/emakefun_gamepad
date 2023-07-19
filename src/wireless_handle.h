#pragma once

#include <log.h>
#include <string.h>

class WirelessHandle {
 public:
  enum ButtonType : uint8_t {
    kButtonJoystick = 0x00,
    kButtonL = 0x01,
    kButtonR = 0x02,
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

  struct JoystickCoordinate {
    int16_t x;
    int16_t y;
  };

  struct GravityAcceleration {
    int16_t x;
    int16_t y;
    int16_t z;
  };

  WirelessHandle() = default;

  virtual ~WirelessHandle() = default;

  virtual void Tick() {
    last_button_state_ = button_state_;
    last_joystick_coordinate_ = joystick_coordinate_;
    last_gravity_acceleration_ = gravity_acceleration_;
  }

  bool NewButtonState() const {
    return (button_state_ ^ last_button_state_) != 0;
  }

  bool NewButtonState(const ButtonType button_type) const {
    return ((button_state_ ^ last_button_state_) & (static_cast<uint16_t>(1) << button_type)) != 0;
  }

  bool ButtonPressed(const ButtonType button_type) const {
    return NewButtonState(button_type) && GetButtonState(button_type);
  }

  bool ButtonReleased(const ButtonType button_type) const {
    return NewButtonState(button_type) && !GetButtonState(button_type);
  }

  bool GetButtonState(const ButtonType button_type) const {
    return (button_state_ & (static_cast<uint16_t>(1) << button_type)) != 0;
  }

  bool NewJoystickCoordinate() const {
    return memcmp(&joystick_coordinate_, &last_joystick_coordinate_, sizeof(JoystickCoordinate)) != 0;
  }

  bool NewGravityAcceleration() const {
    return memcmp(&gravity_acceleration_, &last_gravity_acceleration_, sizeof(GravityAcceleration)) != 0;
  }

  JoystickCoordinate GetJoystickCoordinate() const {
    return joystick_coordinate_;
  }

  GravityAcceleration GetGravityAcceleration() const {
    return gravity_acceleration_;
  }

 protected:
  enum MessageType : uint8_t {
    kButtonState = 0x00,
    kJoystickCoordinate = 0x01,
    kGravityAcceleration = 0x02,
  };

  uint16_t button_state_ = 0;
  uint16_t last_button_state_ = 0;
  JoystickCoordinate joystick_coordinate_{0};
  JoystickCoordinate last_joystick_coordinate_{0};
  GravityAcceleration gravity_acceleration_{0};
  GravityAcceleration last_gravity_acceleration_{0};

 private:
  WirelessHandle(const WirelessHandle&) = delete;

  WirelessHandle& operator=(const WirelessHandle&) = delete;
};