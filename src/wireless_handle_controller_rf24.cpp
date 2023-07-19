#include "wireless_handle_controller_rf24.h"

#include <Arduino.h>
#include <stdlib.h>

#include "log.h"

namespace {
constexpr uint8_t kProtocolVersion = 0x01;
constexpr uint8_t kStartCode = 0xFB;

uint16_t Checksum(const uint8_t* data, const uint8_t length) {
  uint16_t checksum = 0;
  for (uint8_t i = 0; i < length; i++) {
    checksum += data[i];
  }
  return checksum;
}

}  // namespace

WirelessHandleControllerRf24::WirelessHandleControllerRf24() : rf24_(7, 8) {
}

WirelessHandleControllerRf24::~WirelessHandleControllerRf24() = default;

bool WirelessHandleControllerRf24::SetupRf24(const uint8_t channel, const uint8_t address_width, const uint64_t address) {
  rf24_.begin();
  rf24_.setAddressWidth(address_width);
  rf24_.openWritingPipe(address);
  rf24_.setChannel(channel);
  rf24_.setPALevel(RF24_PA_MAX);  // MIN power low rage
  rf24_.setDataRate(RF24_1MBPS);  // Minimum speed
  rf24_.stopListening();          // Stop Receiving and start transmitting
  rf24_.enableDynamicPayloads();
  return true;
}

void WirelessHandleControllerRf24::OnUpdateButtonState() {
  SendPacket((uint8_t[]){kButtonState, button_state_ >> 8, button_state_});
}

void WirelessHandleControllerRf24::OnUpdateJoystickCoordinate() {
  SendPacket((uint8_t[]){kJoystickCoordinate,
                         joystick_coordinate_.x >> 8,
                         joystick_coordinate_.x,
                         joystick_coordinate_.y >> 8,
                         joystick_coordinate_.y});
}

void WirelessHandleControllerRf24::OnUpdateGravityAcceleration() {
  SendPacket((uint8_t[]){kGravityAcceleration,
                         gravity_acceleration_.x >> 8,
                         gravity_acceleration_.x,
                         gravity_acceleration_.y >> 8,
                         gravity_acceleration_.y,
                         gravity_acceleration_.z >> 8,
                         gravity_acceleration_.z});
}

template <size_t length>
void WirelessHandleControllerRf24::SendPacket(const uint8_t (&data)[length]) {
  constexpr uint8_t packet_length = length + 7;
  uint8_t packet_data[packet_length] = {0};
  uint8_t index = 0;
  packet_data[index++] = 0xFB;                   // start_code
  packet_data[index++] = kProtocolVersion;       // protocol_version
  packet_data[index++] = packet_length;          // packet_datalength
  packet_data[index++] = kDeviceType;            // device_type
  packet_data[index++] = kDefaultDeviceAddress;  // device_address
  const uint16_t checksum = Checksum(packet_data, index) + Checksum(data, length);

  packet_data[index++] = checksum >> 8;       // checksum_high
  packet_data[index++] = checksum;            // checksum_low
  memcpy(packet_data + index, data, length);  // data
  rf24_.write(packet_data, packet_length);
}