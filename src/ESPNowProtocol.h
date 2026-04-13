#pragma once

#include <Arduino.h>

class ESPNowProtocol {
public:
  void begin();
  void loop();

  void sendCommand(uint8_t cmd, int32_t value);
};