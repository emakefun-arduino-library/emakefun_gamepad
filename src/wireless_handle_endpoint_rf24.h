#pragma once

#include <stdint.h>

#include "utility/packet_parser.h"
#include "wireless_handle_endpoint.h"

class RF24;

class WirelessHandleEndpointRf24 : public WirelessHandleEndpoint {
 public:
  WirelessHandleEndpointRf24();

  virtual ~WirelessHandleEndpointRf24();

  bool SetupRf24(const uint8_t channel, const uint8_t address_width, const uint64_t address);

  void Tick();

  void SetMessageHandler(const OnMessage& on_message, void* const user_parameter) override {
    on_message_ = on_message;
    user_parameter_ = user_parameter;
  }

 private:
  void HandlePacket(PacketParser::Packet* packet);

  void SendMessage(const Message& message);

  RF24* rf24_ = nullptr;
  PacketParser packet_parser_;
  OnMessage* on_message_ = nullptr;
  void* user_parameter_ = nullptr;
};

namespace {}  // namespace