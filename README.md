# EMakefun Gamepad Arduino 库

## 概述

这个仓库是易创空间手柄(Emakefun Gamepad)的Arduino库，并提供蓝牙(BLE)连接手柄2.4G协议连接手柄的示例:

## 快速开始

### 安装库Arduino文件

请下载该库安装到Arduino的library目录，下载链接为<https://github.com/emakefun-arduino-library/emakefun_gamepad/archive/refs/tags/latest.zip>，并且同时需要安装**MPU6050库**:<https://github.com/emakefun-arduino-library/emakefun_mpu6050> 和**RF24库**:<https://www.arduino.cc/reference/en/libraries/rf24/>

### 示例1：在手柄端打印摇杆和按键值

在手柄上安装好Arduino Nano，请注意检查安装方向是否正确，将示例程序`examples/gamepad/gamepad.ino`下载到Arduino Nano，内容如下:

```c++
#include <Arduino.h>

#include "gamepad.h"

emakefun::Gamepad g_gamepad;
emakefun::GamepadModel g_gamepad_model;

void setup() {
  Serial.begin(115200);
  g_gamepad.Initialize();
  g_gamepad.EnableGyroscope(true);
  g_gamepad.AttachModel(&g_gamepad_model);
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
```

成功运行程序程序后，按下按键或者拨动摇杆，串口将会打印出按下的按键值和摇杆的坐标对应信息，如下：

```log
new button state:5
button released: 5
new button state
new button state:8
button pressed: 8
new button state
new button state:2
button released: 2
joystick coordinate: 108, 125
joystick coordinate: 108, 127
```

该示例程序可用于检测手柄端软硬件是否正常

### 示例2：2.4G连接手柄

(手柄)发送端:`手柄 + Arduino RF-NANO` 或者 `手柄 + Arduino Nano + RF2.4G模块`，作为蓝牙主机

接收端:`Arduino RF-NANO` 或者 `Arduino + RF2.4G模块`，作为蓝牙从机

将示例程序`examples/gamepad_publish_rf24/gamepad_publish_rf24.ino`下载到手柄端, 内容如下:

```c++
#include <Arduino.h>

#include "gamepad.h"
#include "gamepad_publisher_rf24.h"

emakefun::Gamepad g_gamepad;
emakefun::GamepadModel g_gamepad_model;
emakefun::GamepadPublisherRf24 g_gamepad_publisher;

void setup() {
  Serial.begin(115200);
  Serial.println("gamepad initialize");
  g_gamepad.Initialize();
  Serial.println("gamepad initialize done");
  g_gamepad.EnableGyroscope(false);

  Serial.println("gamepad publisher initialize");
  g_gamepad_publisher.Initialize(7, 8, 115, 5, 0x0011000011LL);
  Serial.println("gamepad publisher initialize done");
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
```

将示例程序`examples/gamepad_subscribe_rf24/gamepad_subscribe_rf24.ino`下载到接收端,

*注意*： **Arduino AVR NANO 和 Arduino AVR UNO 的与RF24模块连接的引脚不一样**:

| Arduino AVR UNO | RF2.4 Module |
| --- | --- |
| 10 | CE |
| 9 | CS |

| Arduino AVR NANO | RF2.4 Module |
| --- | --- |
| 7 | CE |
| 8 | CS |

所以烧录的时候选择的Arduino IDE上选择的开发板要选择正确

代码内容如下:

```c++
#include <Arduino.h>

#include "gamepad.h"
#include "gamepad_subscriber_rf24.h"

emakefun::GamepadSubscriberRf24 g_gamepad_subscriber;
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
  g_gamepad_subscriber.Initialize(kRf24CePin, kRf24CsPin, 115, 5, 0x0011000011LL);
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
```

两端成功运行后会自动相连，接收端的串口会打印手柄端的按键值和摇杆坐标值

### 示例3: BLE蓝牙连接手柄

(手柄)发送端:`手柄 + Arduino BLE-NANO` 或者 `手柄 + Arduino Nano + BLE模块`

接收端:`Arduino BLE-NANO` 或者 `Arduino + BLE模块`

开始前先确保接收端的蓝牙配置成从机模式，并处于未连接状态

将示例程序`examples/gamepad_publish_ble/gamepad_publish_ble.ino`下载手柄端, 内容如下:

```c++
#include <Arduino.h>

#include "gamepad.h"
#include "gamepad_publisher_ble.h"

emakefun::Gamepad g_gamepad;
emakefun::GamepadModel g_gamepad_model;
emakefun::GamepadPublisherBle g_gamepad_publisher;

void setup() {
  Serial.begin(115200);
  Serial.println("AT+ROLE=0");
  delay(100);
  Serial.println("AT+CON=75:8B:8C:E4:C2:84");
  delay(100);
  g_gamepad.Initialize();
  g_gamepad.EnableGyroscope(false);
  g_gamepad_publisher.Initialize(Serial);
  g_gamepad.AttachModel(&g_gamepad_model);
  g_gamepad_model.AddObserver(&g_gamepad_publisher);
}

void loop() { g_gamepad.Tick(); }
```

示例代码中`Serial.println("AT+CON=75:8B:8C:E4:C2:84");`中的`75:8B:8C:E4:C2:84`是接收端(从机)的蓝牙MAC地址, 请自行替换。

将示例程序`examples/gamepad_subscibe_ble/gamepad_subscibe_ble.ino`下载到接收端, 内容如下:

```c++
#include <Arduino.h>

#include "gamepad.h"
#include "gamepad_subscriber_ble.h"

emakefun::GamepadSubscriberBle g_gamepad_subscriber;
emakefun::GamepadModel g_gamepad_model;

void setup() {
  Serial.begin(115200);
  g_gamepad_subscriber.Initialize(Serial);
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
      // Serial.print(F("button is being held: "));
      // Serial.println(button_type);
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
```

先上电启动接收端，再上电启动发送端，发送端在启动程序中会连接指定的从机设备，所以需要从机先上电启动，从机蓝牙要配置成BLE从机，且是未连接状态，发送端上电连接成功后，蓝牙指示灯会常亮，接下来接收端的串口会打印手柄端的按键值和摇杆坐标值。接收端不能频繁使用串口打印信息，否则会造成蓝牙数据收发异常，所以接收端的示例中并未打印按住的键值，如下所示。

```c++
if (g_gamepad_model.GetButtonState(button_type)) {
  // Serial.print(F("button is being held: "));
  // Serial.println(button_type);
}
```

## 数据包格式

### 按键数据包格式

**下方表格按数据包字节顺序依次说明！**

| 长度(Byte) | 具体                                                         |
| :--------- | :----------------------------------------------------------- |
| 1          | 开始代码，固定值：0xFB                                       |
| 1          | 协议版本，固定值：0x01                                       |
| 1          | 数据包长度，固定值：0x0A                                     |
| 1          | 设备类型，固定值：0x01                                       |
| 1          | 设备地址，固定值：0x01                                       |
| 2          | 校验和，该数据包所有字节的校验和，大端存储                   |
| 1          | 数据类型，表示传输数据包为按键包，固定值：0x00               |
| 2          | <br/>代表按键状态，大端存储，每个bit代表一个按键，该bit值为1时代表按下，为0时代表未按下。<br><br/>**按键状态具体位数说明：**<br>bit 15 ~ bit9：预留<br>bit 8:  按键D<br>bit 7:  按键C<br>bit 6:  按键B<br>bit 5:  按键A<br>bit 4:  按键Mode<br>bit 3:  按键Select<br>bit 2:  按键R<br>bit 1:  按键L<br>bit 0:  按键Joystick<br><br/>例如：在A按下时，传输的数据包为：按键数据：0x0020<br>校验和：0x0128<br>最终数据包：0xFB，0x01，0x0A，0x01，0x01，0x01，0x28，0x00，0x00，0x20<br><br/>例如：在按键B和按键C都按下时，传输的数据包为：按键数据：0x00C0<br>校验和：0x01C8<br>最终数据包：0xFB，0x01，0x0A，0x01，0x01，0x01，0xC8，0x00，0x00，0xC0<br> |

**下方输出的数据包按按键数据包示例推导即可**

### 摇杆数据包格式

| 长度(Byte) | 具体                                           |
| ---------- | ---------------------------------------------- |
| 1          | 开始代码，固定值：0xFB                         |
| 1          | 协议版本，固定值：0x01                         |
| 1          | 数据包长度，固定值：0x0A                       |
| 1          | 设备类型，固定值：0x01                         |
| 1          | 设备地址，固定值：0x01                         |
| 2          | 校验和，该数据包所有字节的校验和，大端存储     |
| 1          | 数据类型，表示传输数据包为按键包，固定值：0x01 |
| 1          | 摇杆数据的x值                                  |
| 1          | 摇杆数据的y值                                  |

### 重力加速度计数据包格式

| 长度(Byte) | 具体                                           |
| ---------- | ---------------------------------------------- |
| 1          | 开始代码，固定值：0xFB                         |
| 1          | 协议版本，固定值：0x01                         |
| 1          | 数据包长度，固定值：0x0E                       |
| 1          | 设备类型，固定值：0x01                         |
| 1          | 设备地址，固定值：0x01                         |
| 2          | 校验和，该数据包所有字节的校验和，大端存储     |
| 1          | 数据类型，表示传输数据包为按键包，固定值：0x02 |
| 2          | 大端存储，代表重力加速度计的x值                |
| 2          | 大端存储，代表重力加速度计的y值                |
| 2          | 大端存储，代表重力加速度计的z值                |
