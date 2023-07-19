#pragma once

#include "agile_button.h"
#include "mpu6050.h"
#include "wireless_handle.h"

class WirelessHandleController : public WirelessHandle, AgileButton::EventHandler {
 public:
  WirelessHandleController();

  virtual ~WirelessHandleController();

  bool Setup();

  void Tick() override;

 protected:
  virtual void OnUpdateButtonState() {
  }

  virtual void OnUpdateJoystickCoordinate() {
  }

  virtual void OnUpdateGravityAcceleration() {
  }

 private:
  void OnButtonEvent(const AgileButton::Event& event) override;

  Mpu6050 mpu6050_;
  const AgileButton buttons_[kButtonMax];
  uint64_t last_update_button_state_time_ = 0;
  uint64_t last_update_joystick_coordinate_time_ = 0;
  uint64_t last_update_gravity_acceleration_time_ = 0;
};