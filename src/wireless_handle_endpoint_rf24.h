#pragma once

#include <stdint.h>

#include "RF24.h"
#include "utility/packet_parser.h"
#include "wireless_handle.h"

class WirelessHandleEndpointRf24 : public WirelessHandle {
 public:
  WirelessHandleEndpointRf24();

  virtual ~WirelessHandleEndpointRf24();

  bool SetupRf24(const uint8_t channel, const uint8_t address_width, const uint64_t address);

  void Tick() override;

 private:
  void HandlePacket();

  void HandlePacket(PacketParser::Packet* packet);

  RF24 rf24_;
  PacketParser packet_parser_;
};