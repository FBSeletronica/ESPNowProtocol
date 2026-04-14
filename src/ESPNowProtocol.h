#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include "ESPNowTypes.h"

typedef void (*enp_command_cb_t)(uint8_t cmd, int32_t value);
typedef void (*enp_status_cb_t)(int32_t value);

class ESPNowProtocol {
public:
  void begin();
  void loop();

  void setPeer(const uint8_t *mac);

  void sendCommand(uint8_t cmd, int32_t value);
  void sendReliable(uint8_t cmd, int32_t value);

  void sendStatus(int32_t value);

  void onCommand(enp_command_cb_t cb);
  void onStatus(enp_status_cb_t cb);

private:
  static void onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len);

  uint8_t peerAddress[6] = {0};
  uint8_t seqCounter = 0;

  enp_command_cb_t commandCallback = nullptr;
  enp_status_cb_t statusCallback = nullptr;

  bool waitingAck = false;
  uint8_t pendingSeq = 0;
  unsigned long ackStartTime = 0;
  uint8_t retryCount = 0;

  enp_message_t lastMsg;

  static const uint8_t MAX_RETRIES = 3;
  static const uint16_t ACK_TIMEOUT = 100;
};