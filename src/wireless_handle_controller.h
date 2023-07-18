#pragma once

#include "agile_button.h"
#include "mpu6050.h"
#include "wireless_handle.h"

class WirelessHandleController : public WirelessHandle, AgileButton::EventHandler {
 public:
  WirelessHandleController();

  virtual ~WirelessHandleController();

  bool Setup();

  virtual void SetMessageHandler(const OnMessage&, void* user_parameter);

  virtual void SendMessage(const Message& message);

  void Tick();

 private:
  void OnButtonEvent(const AgileButton::Event& event) override;

  Mpu6050 mpu6050_;
  Mpu6050::Acceleration acceleration_{0};
  const AgileButton buttons_[kButtonMax];
  int16_t joystick_coordinate_x_ = 0;
  int16_t joystick_coordinate_y_ = 0;
  uint32_t last_update_acceleration_time_ = 0;
  uint32_t last_update_joystick_coordinate_time_ = 0;
  OnMessage* on_message_ = nullptr;
  void* user_parameter_ = nullptr;
};