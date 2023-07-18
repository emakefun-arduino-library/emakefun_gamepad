#include "wireless_handle_endpoint_rf24.h"

#include "RF24.h"
#include "log.h"

WirelessHandleEndpointRf24::WirelessHandleEndpointRf24() = default;

WirelessHandleEndpointRf24::~WirelessHandleEndpointRf24() {
  delete rf24_;
}

bool WirelessHandleEndpointRf24::SetupRf24(const uint8_t channel, const uint8_t address_width, const uint64_t address) {
  if (rf24_ != nullptr) {
    return false;
  }

  rf24_ = new RF24(7, 8);
  rf24_->begin();
  rf24_->setAddressWidth(address_width);
  rf24_->openReadingPipe(0, address);
  rf24_->setChannel(channel);
  rf24_->setPALevel(RF24_PA_MAX);  // MIN power low rage
  rf24_->setDataRate(RF24_1MBPS);  // Minimum speed
  rf24_->startListening();         // Stop Receiving and start transmitting
  rf24_->enableDynamicPayloads();
  return true;
}

void WirelessHandleEndpointRf24::Tick() {
  if (rf24_ != nullptr) {
    while (rf24_->available()) {
      auto size = rf24_->getDynamicPayloadSize();
      if (size <= 0) {
        break;
      }
      uint8_t* buffer = new uint8_t[size];
      rf24_->read(buffer, size);
      packet_parser_.AppendData(buffer, size);
      // LOG(INFO) << "size: " << size;
      // for (uint8_t i = 0; i < size; i++) {
      //   LOG(INFO) << Log::Hex << Log::ShowBase << Log::SetWidth(4) << Log::SetFill('0') << Log::Internal << buffer[i];
      // }
      delete[] buffer;
    }

    auto packet = packet_parser_.ReadPacket();
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
  const Message message{data[index++], user_parameter_};

  switch (message.message_type) {
    case kButtonDown:
    case kButtonUp:
    case kButtonLongPressBegin:
    case kButtonDuringLongPress:
    case kButtonLongPressEnd: {
      SendMessage(EndpointButtonEventMessage{{packet->source_address}, {message, data[index++]}});
      break;
    }
    case kButtonClick: {
      const ButtonType button_type = data[index++];
      const uint8_t click_count = data[index++];
      SendMessage(EndpointButtonClickMessage{{packet->source_address}, {{message, button_type}, click_count}});
      break;
    }

    case kGravityAccelerationChange: {
      int16_t x = 0;
      int16_t y = 0;
      int16_t z = 0;
      x = data[index++];
      x = (x << 8) | data[index++];
      y = data[index++];
      y = (y << 8) | data[index++];
      z = data[index++];
      z = (z << 8) | data[index++];
      SendMessage(EndpointGravityAccelerationChangeMessage{{packet->source_address}, {message, x, y, z}});
      break;
    }
    case kJoystickCoordinateChange: {
      int16_t x = 0;
      int16_t y = 0;
      x = data[index++];
      x = (x << 8) | data[index++];
      y = data[index++];
      y = (y << 8) | data[index++];
      SendMessage(EndpointJoystickCoordinateChangeMessage{{packet->source_address}, {message, x, y}});
      break;
    }
    default:
      break;
  }
}

void WirelessHandleEndpointRf24::SendMessage(const Message& message) {
  if (on_message_ != nullptr) {
    on_message_(message);
  }
}
