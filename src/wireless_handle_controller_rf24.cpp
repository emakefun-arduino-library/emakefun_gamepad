#include "wireless_handle_controller_rf24.h"

#include <Arduino.h>
#include <stdlib.h>

#include "RF24.h"
#include "log.h"

namespace {

enum : uint8_t {
  kProtocolVersion = 0x01,
};

enum : uint8_t { kStartCode = 0xFB };

uint16_t Checksum(const uint8_t* data, uint8_t length) {
  uint16_t checksum = 0;
  for (uint8_t i = 0; i < length; i++) {
    checksum += data[i];
  }
  return checksum;
}

}  // namespace

WirelessHandleControllerRf24::WirelessHandleControllerRf24() {
}

WirelessHandleControllerRf24::~WirelessHandleControllerRf24() {
  delete rf24_;
  if (packet_ != nullptr) {
    free(packet_);
  }
}

bool WirelessHandleControllerRf24::SetupRf24(const uint8_t channel, const uint8_t address_width, const uint64_t address) {
  if (rf24_ != nullptr) {
    return false;
  }

  rf24_ = new RF24(7, 8);
  rf24_->begin();
  rf24_->setAddressWidth(address_width);
  rf24_->openWritingPipe(address);
  rf24_->setChannel(channel);
  rf24_->setPALevel(RF24_PA_MAX);  // MIN power low rage
  rf24_->setDataRate(RF24_1MBPS);  // Minimum speed
  rf24_->stopListening();          // Stop Receiving and start transmitting
  rf24_->enableDynamicPayloads();
  return true;
}

void WirelessHandleControllerRf24::SetSourceAddress(const uint8_t device_address) {
  device_address_ = device_address;
}

void WirelessHandleControllerRf24::SendMessage(const Message& message) {
  WirelessHandleController::SendMessage(message);

  switch (message.message_type) {
    case kButtonDown:
    case kButtonUp:
    case WirelessHandle::kButtonLongPressBegin:
    case WirelessHandle::kButtonDuringLongPress:
    case WirelessHandle::kButtonLongPressEnd: {
      MakePacket((uint8_t[]){message.message_type, GetButtonType(message)});
      break;
    }
    case kButtonClick: {
      MakePacket((uint8_t[]){message.message_type, GetButtonType(message), GetButtonClickCount(message)});
      break;
    }
    case WirelessHandle::kGravityAccelerationChange: {
      MakePacket((uint8_t[]){message.message_type,
                             GetGravityAccelerationX(message) >> 8,
                             GetGravityAccelerationX(message),
                             GetGravityAccelerationY(message) >> 8,
                             GetGravityAccelerationY(message),
                             GetGravityAccelerationZ(message) >> 8,
                             GetGravityAccelerationZ(message)});
      break;
    }
    case WirelessHandle::kJoystickCoordinateChange: {
      MakePacket((uint8_t[]){message.message_type,
                             GetJoystickCoordinateX(message) >> 8,
                             GetJoystickCoordinateX(message),
                             GetJoystickCoordinateY(message) >> 8,
                             GetJoystickCoordinateY(message)});
      break;
    }
    default: {
      return;
    }
  }

  SendPacketOnRf24();
}

void WirelessHandleControllerRf24::SendPacketOnRf24() {
  if (rf24_ == nullptr) {
    return;
  }
  rf24_->write(packet_, packet_length_);
}

template <size_t length>
void WirelessHandleControllerRf24::MakePacket(const uint8_t (&data)[length]) {
  packet_length_ = length + 7;
  packet_ = reinterpret_cast<uint8_t*>(realloc(packet_, packet_length_));
  uint8_t index = 0;
  packet_[index++] = 0xFB;              // start_code
  packet_[index++] = kProtocolVersion;  // protocol_version
  packet_[index++] = packet_length_;    // packet_length
  packet_[index++] = kDeviceType;       // device_type
  packet_[index++] = device_address_;   // device_address
  const uint16_t checksum = Checksum(packet_, index) + Checksum(data, length);

  packet_[index++] = checksum >> 8;       // checksum_high
  packet_[index++] = checksum;            // checksum_low
  memcpy(packet_ + index, data, length);  // data
}