#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include "ESPNowTypes.h"

#define ENP_MAX_PEERS 10

typedef struct {
  uint8_t id;
  uint8_t mac[6];
  unsigned long lastSeen;
  bool active;
  int8_t rssi;
} enp_peer_t;

typedef void (*enp_receive_cb_t)(
  uint8_t src,
  uint8_t id,
  const uint8_t *data,
  uint8_t len,
  int8_t rssi
);

class ESPNowProtocol {
public:
  void begin();
  void loop();

  void setNodeId(uint8_t id);

  void addPeer(uint8_t id, const uint8_t *mac);

  void enableAutoDiscovery(bool enable);

  void send(uint8_t dest, uint8_t id, const uint8_t *data, uint8_t len);
  void sendReliable(uint8_t dest, uint8_t id, const uint8_t *data, uint8_t len);

  void onReceive(enp_receive_cb_t cb);

  int8_t getPeerRSSI(uint8_t id);
  uint8_t getLinkQuality(uint8_t id);

private:
  static void onReceiveInternal(const esp_now_recv_info_t *info, const uint8_t *data, int len);

  uint8_t nodeId = 0;
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

  enp_peer_t peers[ENP_MAX_PEERS];
  uint8_t peerCount = 0;

  bool autoDiscovery = false;
  unsigned long lastHello = 0;
  static const uint16_t HELLO_INTERVAL = 3000;

  unsigned long lastHeartbeat = 0;
  static const uint16_t HEARTBEAT_INTERVAL = 2000;
  static const uint16_t PEER_TIMEOUT = 5000;

  bool isQueueEmpty();
  void pushQueue(const enp_packet_t &pkt);
  bool popQueue(enp_packet_t &pkt);

  const uint8_t* findPeerMac(uint8_t id);
};