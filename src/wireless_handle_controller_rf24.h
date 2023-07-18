#pragma once

#include "agile_button.h"
#include "mpu6050.h"
#include "wireless_handle_controller.h"

class RF24;

class WirelessHandleControllerRf24 : public WirelessHandleController {
 public:
  WirelessHandleControllerRf24();

  virtual ~WirelessHandleControllerRf24();

  bool SetupRf24(const uint8_t channel, const uint8_t address_width, const uint64_t address);

  void SetSourceAddress(const uint8_t device_address);

  virtual void SendMessage(const Message& message);

 private:
  template <size_t size>
  void MakePacket(const uint8_t (&data)[size]);

  void SendPacketOnRf24();

  RF24* rf24_ = nullptr;
  uint8_t* packet_ = nullptr;
  uint8_t packet_length_ = 0;
  uint8_t device_address_ = kDefaultDeviceAddress;
};