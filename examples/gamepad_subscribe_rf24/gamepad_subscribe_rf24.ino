#include <Arduino.h>

#include "gamepad.h"
#include "gamepad_subscriber_rf24.h"

emakefun::GamepadModel g_gamepad_model;

#if defined(ARDUINO_AVR_UNO)
constexpr uint8_t kRf24CePin = 10;
constexpr uint8_t kRf24CsPin = 9;
#elif defined(ARDUINO_AVR_NANO)
constexpr uint8_t kRf24CePin = 7;
constexpr uint8_t kRf24CsPin = 8;
#else
#error "unsupported board type"
#endif

emakefun::GamepadSubscriberRf24 g_gamepad_subscriber(kRf24CePin, kRf24CsPin);

void setup() {
  Serial.begin(115200);
#if defined(ARDUINO_AVR_UNO)
  Serial.println("Arduino AVR UNO");
#elif defined(ARDUINO_AVR_NANO)
  Serial.println("Arduino AVR NANO");
#endif
  Serial.print("rf24 ce pin: ");
  Serial.print(kRf24CePin);
  Serial.print(", cs pin: ");
  Serial.println(kRf24CsPin);
  g_gamepad_subscriber.Initialize(115, 5, 0x0011000011LL);
  g_gamepad_subscriber.AttachModel(&g_gamepad_model);
  Serial.println("setup done");
}

void loop() {
  g_gamepad_subscriber.Tick();
  if (g_gamepad_model.NewButtonState()) {
    Serial.println(F("new button state"));
  }

  for (uint8_t button_type = emakefun::GamepadModel::kButtonJoystick; button_type < emakefun::GamepadModel::kButtonMax;
       button_type++) {
    if (g_gamepad_model.NewButtonState(button_type)) {
      Serial.print(F("new button state:"));
      Serial.println(button_type);
    }

    if (g_gamepad_model.ButtonPressed(button_type)) {
      Serial.print(F("button pressed: "));
      Serial.println(button_type);
    }

    if (g_gamepad_model.ButtonReleased(button_type)) {
      Serial.print(F("button released: "));
      Serial.println(button_type);
    }

    if (g_gamepad_model.GetButtonState(button_type)) {
      Serial.print(F("button is being held: "));
      Serial.println(button_type);
    }
  }

  if (g_gamepad_model.NewJoystickCoordinate()) {
    Serial.print(F("joystick coordinate: "));
    Serial.print(g_gamepad_model.GetJoystickCoordinate().x);
    Serial.print(F(", "));
    Serial.println(g_gamepad_model.GetJoystickCoordinate().y);
  }

  if (g_gamepad_model.NewGravityAcceleration()) {
    Serial.print(F("gravity acceleration: "));
    Serial.print(g_gamepad_model.GetGravityAcceleration().x);
    Serial.print(F(", "));
    Serial.print(g_gamepad_model.GetGravityAcceleration().y);
    Serial.print(F(", "));
    Serial.println(g_gamepad_model.GetGravityAcceleration().z);
  }
}