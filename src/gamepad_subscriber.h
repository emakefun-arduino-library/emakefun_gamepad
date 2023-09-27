#pragma once

#include "gamepad_model.h"
#include "utility/packet_parser.h"

namespace emakefun {
class GamepadSubscriber {
 public:
  virtual ~GamepadSubscriber() = default;
  void AttachModel(GamepadModel* const model);

 protected:
  void Process(const uint8_t* data, const uint32_t length);

 private:
  void HandlePacket();
  void HandlePacket(PacketParser::Packet* packet);

 private:
  GamepadModel* model_ = nullptr;
  PacketParser packet_parser_;
};
}  // namespace emakefun