#pragma once

#include "RF24.h"
#include "gamepad_model.h"
#include "gamepad_subscriber.h"
#include "utility/packet_parser.h"

namespace emakefun {
class GamepadRf24Subscriber : public GamepadSubscriber {
 public:
  GamepadRf24Subscriber();
  bool Initialize(const uint8_t channel, const uint8_t address_width, const uint64_t address);
  void Tick();

 private:
  RF24 rf24_;
};
}  // namespace emakefun
