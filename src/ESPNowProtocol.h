/*
 * ESPNowProtocol.h
 *
 * Lightweight communication layer built on top of ESP-NOW for ESP32 devices.
 * The library provides logical node addressing, optional automatic peer
 * discovery, non-blocking reliable delivery with ACK/retry, and basic link
 * diagnostics such as RSSI and link quality estimation.
 *
 * Author: Fabio Souza
 * License: MIT
 */

#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include "ESPNowTypes.h"

#define ENP_MAX_PEERS 10

// Runtime metadata maintained for each known peer in the network.
typedef struct {
  uint8_t id;
  uint8_t mac[6];
  unsigned long lastSeen;
  bool active;
  int8_t rssi;
} enp_peer_t;

// User callback signature for application payload reception.
typedef void (*enp_receive_cb_t)(
  uint8_t src,
  uint8_t id,
  const uint8_t *data,
  uint8_t len,
  int8_t rssi
);

class ESPNowProtocol {
public:
  // Initializes Wi-Fi in station mode and prepares ESP-NOW callbacks/peers.
  void begin();

  // Processes retries, periodic HELLO/HEARTBEAT frames, and peer timeouts.
  void loop();

  // Sets the logical identifier used as packet source address.
  void setNodeId(uint8_t id);

  // Registers a peer using a logical node id and its ESP-NOW MAC address.
  void addPeer(uint8_t id, const uint8_t *mac);

  // Enables or disables broadcast-based peer discovery.
  void enableAutoDiscovery(bool enable);

  // Sends a packet immediately without reliability tracking.
  void send(uint8_t dest, uint8_t id, const uint8_t *data, uint8_t len);

  // Queues a packet for ACK-based delivery with automatic retry.
  void sendReliable(uint8_t dest, uint8_t id, const uint8_t *data, uint8_t len);

  // Registers the application callback invoked for incoming data packets.
  void onReceive(enp_receive_cb_t cb);

  // Returns the most recently observed RSSI for a peer.
  int8_t getPeerRSSI(uint8_t id);

  // Converts RSSI into a coarse percentage-based quality estimate.
  uint8_t getLinkQuality(uint8_t id);

private:
  // Static trampoline used by the ESP-NOW receive callback API.
  static void onReceiveInternal(const esp_now_recv_info_t *info, const uint8_t *data, int len);

  uint8_t nodeId = 0;
  uint8_t seqCounter = 0;

  enp_receive_cb_t receiveCallback = nullptr;

  // Tracks the packet currently waiting for an ACK.
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

  // Ring-buffer helpers for reliable outbound traffic.
  bool isQueueEmpty();
  void pushQueue(const enp_packet_t &pkt);
  bool popQueue(enp_packet_t &pkt);

  // Looks up the ESP-NOW MAC address associated with a logical node id.
  const uint8_t* findPeerMac(uint8_t id);
};
