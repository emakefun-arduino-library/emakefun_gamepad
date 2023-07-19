#include "wireless_handle_endpoint_rf24.h"

#include "RF24.h"
#include "log.h"

WirelessHandleEndpointRf24::WirelessHandleEndpointRf24() : rf24_(7, 8) {
}

WirelessHandleEndpointRf24::~WirelessHandleEndpointRf24() = default;

bool WirelessHandleEndpointRf24::SetupRf24(const uint8_t channel, const uint8_t address_width, const uint64_t address) {
  rf24_.begin();
  rf24_.setAddressWidth(address_width);
  rf24_.openReadingPipe(0, address);
  rf24_.setChannel(channel);
  rf24_.setPALevel(RF24_PA_MAX);  // MIN power low rage
  rf24_.setDataRate(RF24_1MBPS);  // Minimum speed
  rf24_.startListening();         // Stop Receiving and start transmitting
  rf24_.enableDynamicPayloads();
  return true;
}

void WirelessHandleEndpointRf24::Tick() {
  WirelessHandle::Tick();

  while (rf24_.available()) {
    auto size = rf24_.getDynamicPayloadSize();
    if (size <= 0) {
      break;
    }
    uint8_t* buffer = new uint8_t[size];
    rf24_.read(buffer, size);
    if (size != packet_parser_.AppendData(buffer, size)) {
      LOG(ERROR) << "buffer is full and drop " << size << " bytes";
    }
    // LOG(INFO) << "size: " << size;
    // for (uint8_t i = 0; i < size; i++) {
    //   LOG(INFO) << Log::Hex << Log::ShowBase << Log::SetWidth(4) << Log::SetFill('0') << Log::Internal << buffer[i];
    // }
    delete[] buffer;
    HandlePacket();
  }
  HandlePacket();
}

void WirelessHandleEndpointRf24::HandlePacket() {
  PacketParser::Packet* packet = nullptr;
  while ((packet = packet_parser_.ReadPacket()) != nullptr) {
    HandlePacket(packet);
    packet_parser_.ReleasePacket(packet);
  }
}

void WirelessHandleEndpointRf24::HandlePacket(PacketParser::Packet* packet) {
  if (packet == nullptr || packet->data == nullptr || packet->data_length == 0) {
    return;
  }
  uint8_t index = 0;
  const uint8_t* const data = packet->data;
  const MessageType message_type = data[index++];
  switch (message_type) {
    case kButtonState:
      button_state_ = data[index++];
      button_state_ = (button_state_ << 8) | data[index++];
      break;

    case kJoystickCoordinate: {
      joystick_coordinate_.x = data[index++];
      joystick_coordinate_.x = (joystick_coordinate_.x << 8) | data[index++];
      joystick_coordinate_.y = data[index++];
      joystick_coordinate_.y = (joystick_coordinate_.y << 8) | data[index++];
      break;
    }

    case kGravityAcceleration: {
      gravity_acceleration_.x = data[index++];
      gravity_acceleration_.x = (gravity_acceleration_.x << 8) | data[index++];
      gravity_acceleration_.y = data[index++];
      gravity_acceleration_.y = (gravity_acceleration_.y << 8) | data[index++];
      gravity_acceleration_.z = data[index++];
      gravity_acceleration_.z = (gravity_acceleration_.z << 8) | data[index++];
      break;
    }

    default: {
      break;
    }
  }
}