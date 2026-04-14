# ESPNowProtocol

ESPNowProtocol is a lightweight communication layer built on top of ESP-NOW for ESP32.

It provides a simple and reliable way to exchange data between devices, supporting:
- Generic payload (no fixed protocol)
- Reliable communication (ACK + retry)
- Message queue (non-blocking)
- Automatic peer discovery

---

## Features

- Generic message transport (no command/value limitation)
- Reliable messaging with ACK and retry
- Internal queue for sequential transmission
- Callback-based reception
- Automatic peer management

---

## Installation

1. Copy the library to your Arduino libraries folder:  ~/Arduino/libraries/ESPNowProtocol

2. Restart Arduino IDE

---

## Basic Usage

### Sender

```cpp
#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

uint8_t peer[] = {0x10,0x52,0x1C,0x68,0x15,0x28};

void setup()
{
  Serial.begin(115200);

  protocol.begin();
  protocol.setPeer(peer);
}

void loop()
{
  protocol.loop();

  int value = 42;
  protocol.sendReliable(1, (uint8_t*)&value, sizeof(value));
}
```
### Receiver
```cpp
#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

void onData(uint8_t id, const uint8_t *data, uint8_t len)
{
  if (id == 1)
  {
    int value;
    memcpy(&value, data, sizeof(value));

    Serial.println(value);
  }
}

void setup()
{
  Serial.begin(115200);

  protocol.begin();
  protocol.onReceive(onData);
}

void loop()
{
  protocol.loop();
}
```
## Message Format

Each message contains:

- type → internal (DATA / ACK)
- id → user-defined identifier
- len → payload size
- payload → raw data (up to 32 bytes)
- seq → sequence number (used for reliability)

## Important Notes
- protocol.loop() must be called frequently (non-blocking design)
- Avoid using long delay() calls
- Maximum payload size: 32 bytes

## Example Payload
```cpp
struct MotorCommand {
  int speed;
  int direction;
};

MotorCommand cmd = {100, 1};

protocol.sendReliable(1, (uint8_t*)&cmd, sizeof(cmd));
```

---
## Roadmap
- Multi-device support
- Broadcast mode
- Message priority
- Diagnostics and metrics

## License

MIT License