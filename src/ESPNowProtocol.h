#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include "ESPNowTypes.h"

typedef void (*enp_command_cb_t)(uint8_t cmd, int32_t value);

class ESPNowProtocol {
public:
  void begin();
  void loop();

  void setPeer(const uint8_t *mac);

  void sendCommand(uint8_t cmd, int32_t value);

  void onCommand(enp_command_cb_t cb);

private:
  static void onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len);

  uint8_t peerAddress[6] = {0};
  uint8_t seqCounter = 0;

  enp_command_cb_t commandCallback = nullptr;
};