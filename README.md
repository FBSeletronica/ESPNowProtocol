# ESPNowProtocol

## ⭐ Star this project

If this library helped you, consider giving it a star!

![CI](https://github.com/FBSeletronica/ESPNowProtocol/actions/workflows/ci.yml/badge.svg)
![License](https://img.shields.io/badge/license-MIT-blue.svg)
![GitHub release](https://img.shields.io/github/v/release/FBSeletronica/ESPNowProtocol)
[![Arduino Library Manager](https://www.ardu-badge.com/badge/ESPNowProtocol.svg)](https://www.ardu-badge.com/ESPNowProtocol)

ESPNowProtocol is a lightweight and scalable communication layer built on top of ESP-NOW for ESP32.

🚀 Designed for reliable, scalable communication between multiple ESP32 devices using ESP-NOW.

It provides a complete solution for device-to-device communication, including:
- Generic payload messaging
- Reliable delivery (ACK + retry)
- Message queue (non-blocking)
- Multi-device networking
- Automatic peer discovery
- Device lifecycle management (online/offline)
- Link diagnostics (RSSI + quality)

---

## 🚀 Quick Start

### 1. Install the Library

Install via Arduino Library Manager:

- Open Arduino IDE
- Go to **Sketch → Include Library → Manage Libraries...**
- Search for **ESPNowProtocol**
- Click **Install**

---

### 2. Select Your Board

- ESP32
- ESP32-S2
- ESP32-S3
- ESP32-C3

💡 Requires two boards (sender + receiver)

---

### 3. Upload Sender

Open:

```
File → Examples → ESPNowProtocol → basic_sender
```

Upload to your first board.

---

### 4. Upload Receiver

Open:

```
File → Examples → ESPNowProtocol → basic_receiver
```

Upload to another board.

---

### 5. Open Serial Monitor

Set baud rate to:

```
115200
```

---

### 6. Done!

Your devices should now be communicating via ESP-NOW 🎉

---

💡 Tip: Start with `basic_sender` and `basic_receiver` before trying advanced features.

---

## 📦 Use Cases

- Wireless sensor networks (ESP32 clusters)
- Remote control systems (low latency)
- IoT device communication
- Real-time data exchange

## 📡 How it works

ESP32 (Sender) → ESP-NOW → ESP32 (Receiver)

Low-latency, connectionless communication without Wi-Fi infrastructure.

## ✨ Features

- Generic payload (no fixed protocol)
- Reliable communication with ACK and retry
- Internal message queue (non-blocking)
- Addressing system (src/dest)
- Multi-peer support
- Broadcast messaging
- Auto discovery (HELLO)
- Heartbeat system
- Peer timeout detection (offline detection)
- RSSI measurement per peer
- Link quality estimation

---

## Supported SoCs

| SoC       | Status       |
|----------|-------------|
| ESP32     | ✅ Tested     |
| ESP32-S2  | ✅ Tested     |
| ESP32-C3  | ✅ Tested     |
| ESP32-S3  | ⚠️ Untested   |
| ESP32-C6  | ⚠️ Untested   |

---

## Installation

### Arduino Library Manager (recommended)

- Open Arduino IDE  
- Go to **Sketch → Include Library → Manage Libraries...**  
- Search for **ESPNowProtocol**  
- Click **Install**

---

### Manual Installation (alternative)

1. Copy the library to your Arduino libraries folder:  
   ~/Arduino/libraries/ESPNowProtocol  
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
  protocol.setNodeId(1);

  protocol.addPeer(2, peer);
}

void loop()
{
  protocol.loop();

  int value = 42;
  protocol.sendReliable(2, 1, (uint8_t*)&value, sizeof(value));
}
```
### Receiver
```cpp
#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

void onData(uint8_t src, uint8_t id, const uint8_t *data, uint8_t len, int8_t rssi)
{
  int value;
  memcpy(&value, data, sizeof(value));

  Serial.print("From: ");
  Serial.print(src);
  Serial.print(" | RSSI: ");
  Serial.println(rssi);

  Serial.println(value);
}

void setup()
{
  Serial.begin(115200);

  protocol.begin();
  protocol.setNodeId(2);

  protocol.onReceive(onData);
}

void loop()
{
  protocol.loop();
}
```
## Addressing Model

Each device has a logical ID:
```
src → sender ID  
dest → target ID (255 = broadcast)
```

## Message Format
Each packet contains:

- type → DATA / ACK / HELLO / HEARTBEAT
- src → sender ID
- dest → destination ID
- id → user-defined message ID
- len → payload size
- payload → raw data (max 32 bytes)
- seq → sequence number (reliability)

## Auto Discovery

Devices automatically discover each other using HELLO messages:
```cpp
protocol.enableAutoDiscovery(true);
```

## Lifecycle Management

The library automatically tracks device status:

Peer online detection
Heartbeat messages
Peer timeout (offline detection)

Example output:
```
[ENP] New peer: 23
[ENP] Peer offline: 23
```

## Link Diagnostics

Each received packet includes RSSI:
```cpp
int8_t rssi;
```

You can also query:
```cpp
protocol.getPeerRSSI(id);
protocol.getLinkQuality(id);
```

## RSSI Reference

| RSSI (dBm) | Quality   |
| ---------- | --------- |
| -30        | Excellent |
| -50        | Very good |
| -70        | Good      |
| -85        | Weak      |
| -95        | Poor      |


## Examples

The library includes several examples:
```
examples/
├── basic_sender
├── basic_receiver
├── addressing
├── broadcast
├── auto_discovery
└── network_monitor
```

## Recommended starting points
- `basic_sender` / `basic_receiver` → minimal usage
- `auto_discovery` → automatic network formation
- `network_monitor` → full diagnostics (RSSI + peers)

## Important Notes
- ⚠️ `protocol.loop()` must run continuously
- Avoid long blocking delays
- Maximum payload size: 32 bytes
- Each device must have a unique nodeId


## Example Payload
```cpp
struct MotorCommand {
  int speed;
  int direction;
};

MotorCommand cmd = {100, 1};

protocol.sendReliable(2, 1, (uint8_t*)&cmd, sizeof(cmd));
```

## Version

Current version: v1.2.0

## Roadmap
- Peer event callbacks (online/offline)
- Link metrics (packet loss, latency)
- Automatic best-peer selection
- Mesh routing (multi-hop)

## Contributing

See [CONTRIBUTING.md](./CONTRIBUTING.md) for details.

## Releases

See the [Release Checklist](./RELEASE_CHECKLIST.md) for the full release process.

## License

MIT License


