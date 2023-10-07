#include <Arduino.h>

#include "gamepad.h"
#include "gamepad_rf24_publisher.h"

emakefun::Gamepad g_gamepad;
emakefun::GamepadModel g_gamepad_model;
emakefun::GamepadRf24Publisher g_gamepad_publisher;

void setup() {
  Serial.begin(115200);
  g_gamepad.Initialize();
  g_gamepad.EnableGyroscope(false);
  g_gamepad_publisher.Initialize(115, 5, 0x0011000011LL);
  g_gamepad.AttachModel(&g_gamepad_model);
  g_gamepad_model.AddObserver(&g_gamepad_publisher);
  Serial.println("setup done");
}

void loop() {
  g_gamepad.Tick();
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