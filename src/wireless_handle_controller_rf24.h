#pragma once

#include "RF24.h"
#include "wireless_handle_controller.h"

class WirelessHandleControllerRf24 : public WirelessHandleController {
 public:
  WirelessHandleControllerRf24();

  virtual ~WirelessHandleControllerRf24();

  bool SetupRf24(const uint8_t channel, const uint8_t address_width, const uint64_t address);

  void OnUpdateButtonState() override;

  void OnUpdateJoystickCoordinate() override;

  void OnUpdateGravityAcceleration() override;

 private:
  template <size_t size>
  void SendPacket(const uint8_t (&data)[size]);

  RF24 rf24_;
};