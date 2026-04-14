# ESPNowProtocol

![CI](https://github.com/FBSeletronica/ESPNowProtocol/actions/workflows/ci.yml/badge.svg)
![License](https://img.shields.io/badge/license-MIT-blue.svg)

ESPNowProtocol is a lightweight and scalable communication layer built on top of ESP-NOW for ESP32.

It provides a complete solution for device-to-device communication, including:
- Generic payload messaging
- Reliable delivery (ACK + retry)
- Message queue (non-blocking)
- Multi-device networking
- Automatic peer discovery
- Device lifecycle management (online/offline)
- Link diagnostics (RSSI + quality)

---

## Features

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
- `protocol.loop()` must run continuously
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

## License

MIT License

## Contributing

Contributions are welcome! This project follows a simple and reliable workflow to ensure code quality and consistency.

---

### Workflow Overview

1. Fork the repository
2. Create a new branch from `main`
3. Make your changes
4. Open a Pull Request

---

### Development Setup

1. Clone your fork:

```
git clone https://github.com/FBSeletronica/ESPNowProtocol.git
cd ESPNowProtocol
```

2. Make sure you have:

* Arduino CLI installed
* ESP32 core installed

---

### Testing

Before submitting a Pull Request:

* Ensure all examples compile
* Run Arduino Lint:

```
arduino-lint --compliance strict --library-manager submit
```

---

### CI Behavior

This project uses GitHub Actions for validation:

* **Lint** runs on every commit
* **Build** runs:

  * On all Pull Requests
  * On commits containing `[build]` or `[ci]`

To force a full build:

```
git commit -m "fix: adjust protocol [build]"
```

---

### Commit Convention

This project uses **Conventional Commits**:

```
feat: add new feature
fix: resolve bug
docs: update documentation
chore: maintenance tasks
```

Only `feat` and `fix` should represent changes that may affect a release.

---

### Pull Request Guidelines

* Keep PRs small and focused
* Do not include unrelated changes
* Ensure CI passes before requesting review
* Update documentation if needed

---

###  Releases

Releases are managed by the project maintainer.

Contributors do **not** need to create tags or releases.

When a new version is ready, the maintainer will:

1. Update the version in `library.properties`
2. Run Arduino Lint:

   ```
   arduino-lint --compliance strict --library-manager submit
   ```
3. Create a Git tag:

   ```
   git tag 1.0.0
   git push origin 1.0.0
   ```
4. Create a release on GitHub (optional but recommended)

---

### Questions

If you have any questions, feel free to open an issue.

---

Thank you for contributing!
