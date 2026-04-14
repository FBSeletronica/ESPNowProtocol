#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include "ESPNowTypes.h"

typedef void (*enp_receive_cb_t)(
  uint8_t src,
  uint8_t id,
  const uint8_t *data,
  uint8_t len
);

class ESPNowProtocol {
public:
  void begin();
  void loop();

  void setPeer(const uint8_t *mac);
  void setNodeId(uint8_t id);

  void send(uint8_t dest, uint8_t id, const uint8_t *data, uint8_t len);
  void sendReliable(uint8_t dest, uint8_t id, const uint8_t *data, uint8_t len);

  void onReceive(enp_receive_cb_t cb);

private:
  static void onReceiveInternal(const esp_now_recv_info_t *info, const uint8_t *data, int len);

  uint8_t nodeId = 0;
  uint8_t peerAddress[6] = {0};
  uint8_t seqCounter = 0;

  enp_receive_cb_t receiveCallback = nullptr;

  bool waitingAck = false;
  uint8_t pendingSeq = 0;
  unsigned long ackStartTime = 0;
  uint8_t retryCount = 0;

  enp_packet_t lastPacket;

  static const uint8_t MAX_RETRIES = 3;
  static const uint16_t ACK_TIMEOUT = 100;

  static const uint8_t QUEUE_SIZE = 5;

  enp_packet_t queue[QUEUE_SIZE];

  uint8_t queueHead = 0;
  uint8_t queueTail = 0;
  bool queueFull = false;

  bool isQueueEmpty();
  void pushQueue(const enp_packet_t &pkt);
  bool popQueue(enp_packet_t &pkt);
};