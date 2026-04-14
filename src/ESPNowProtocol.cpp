#include "ESPNowProtocol.h"

static ESPNowProtocol* instance = nullptr;

// broadcast MAC
static const uint8_t broadcastMac[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

// --------------------------------------------------

bool ESPNowProtocol::isQueueEmpty()
{
  return (!queueFull && (queueHead == queueTail));
}

void ESPNowProtocol::pushQueue(const enp_packet_t &pkt)
{
  queue[queueHead] = pkt;

  if (queueFull)
  {
    queueTail = (queueTail + 1) % QUEUE_SIZE;
  }

  queueHead = (queueHead + 1) % QUEUE_SIZE;
  queueFull = (queueHead == queueTail);
}

bool ESPNowProtocol::popQueue(enp_packet_t &pkt)
{
  if (isQueueEmpty()) return false;

  pkt = queue[queueTail];
  queueFull = false;

  queueTail = (queueTail + 1) % QUEUE_SIZE;

  return true;
}

// --------------------------------------------------

const uint8_t* ESPNowProtocol::findPeerMac(uint8_t id)
{
  for (int i = 0; i < peerCount; i++)
  {
    if (peers[i].id == id)
    {
      return peers[i].mac;
    }
  }
  return nullptr;
}

// --------------------------------------------------

void ESPNowProtocol::begin()
{
  instance = this;

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("[ENP] init failed");
    return;
  }

  // register broadcast peer
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (!esp_now_is_peer_exist(broadcastMac))
  {
    esp_now_add_peer(&peerInfo);
  }

  esp_now_register_recv_cb(ESPNowProtocol::onReceiveInternal);
}

// --------------------------------------------------

void ESPNowProtocol::setNodeId(uint8_t id)
{
  nodeId = id;
}

// --------------------------------------------------

void ESPNowProtocol::addPeer(uint8_t id, const uint8_t *mac)
{
  if (peerCount >= ENP_MAX_PEERS) return;

  peers[peerCount].id = id;
  memcpy(peers[peerCount].mac, mac, 6);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, mac, 6);

  if (!esp_now_is_peer_exist(mac))
  {
    esp_now_add_peer(&peerInfo);
  }

  peerCount++;
}

// --------------------------------------------------

void ESPNowProtocol::enableAutoDiscovery(bool enable)
{
  autoDiscovery = enable;
}

// --------------------------------------------------

void ESPNowProtocol::loop()
{
  // envio da fila
  if (!waitingAck)
  {
    enp_packet_t pkt;

    if (popQueue(pkt))
    {
      lastPacket = pkt;

      if (pkt.dest == 255)
      {
        esp_now_send(broadcastMac, (uint8_t *)&lastPacket, sizeof(lastPacket));
      }
      else
      {
        const uint8_t *mac = findPeerMac(pkt.dest);
        if (mac)
        {
          esp_now_send(mac, (uint8_t *)&lastPacket, sizeof(lastPacket));
        }
      }

      pendingSeq = lastPacket.seq;
      waitingAck = true;
      retryCount = 0;
      ackStartTime = millis();
    }
  }

  // retry
  if (waitingAck)
  {
    if (millis() - ackStartTime > ACK_TIMEOUT)
    {
      if (retryCount < MAX_RETRIES)
      {
        retryCount++;

        if (lastPacket.dest == 255)
        {
          esp_now_send(broadcastMac, (uint8_t *)&lastPacket, sizeof(lastPacket));
        }
        else
        {
          const uint8_t *mac = findPeerMac(lastPacket.dest);
          if (mac)
          {
            esp_now_send(mac, (uint8_t *)&lastPacket, sizeof(lastPacket));
          }
        }

        ackStartTime = millis();
      }
      else
      {
        waitingAck = false;
      }
    }
  }

  // HELLO broadcast
  if (autoDiscovery)
  {
    if (millis() - lastHello > HELLO_INTERVAL)
    {
      enp_packet_t pkt = {};

      pkt.type = ENP_MSG_HELLO;
      pkt.src  = nodeId;
      pkt.dest = 255;
      pkt.seq  = seqCounter++;

      esp_now_send(broadcastMac, (uint8_t *)&pkt, sizeof(pkt));

      lastHello = millis();
    }
  }
}

// --------------------------------------------------

void ESPNowProtocol::send(uint8_t dest, uint8_t id, const uint8_t *data, uint8_t len)
{
  if (len > ENP_MAX_PAYLOAD) return;

  enp_packet_t pkt = {};

  pkt.type = ENP_MSG_DATA;
  pkt.src  = nodeId;
  pkt.dest = dest;
  pkt.id   = id;
  pkt.len  = len;

  memcpy(pkt.payload, data, len);

  pkt.seq = seqCounter++;

  if (dest == 255)
  {
    esp_now_send(broadcastMac, (uint8_t *)&pkt, sizeof(pkt));
  }
  else
  {
    const uint8_t *mac = findPeerMac(dest);
    if (mac)
    {
      esp_now_send(mac, (uint8_t *)&pkt, sizeof(pkt));
    }
  }
}

// --------------------------------------------------

void ESPNowProtocol::sendReliable(uint8_t dest, uint8_t id, const uint8_t *data, uint8_t len)
{
  if (len > ENP_MAX_PAYLOAD) return;

  enp_packet_t pkt = {};

  pkt.type = ENP_MSG_DATA;
  pkt.src  = nodeId;
  pkt.dest = dest;
  pkt.id   = id;
  pkt.len  = len;

  memcpy(pkt.payload, data, len);

  pkt.seq = seqCounter++;

  pushQueue(pkt);
}

// --------------------------------------------------

void ESPNowProtocol::onReceive(enp_receive_cb_t cb)
{
  receiveCallback = cb;
}

// --------------------------------------------------

void ESPNowProtocol::onReceiveInternal(const esp_now_recv_info_t *info, const uint8_t *data, int len)
{
  if (len < sizeof(enp_packet_t)) return;
  if (!instance) return;

  enp_packet_t pkt;
  memcpy(&pkt, data, sizeof(pkt));

  // filtro destino
  if (pkt.dest != instance->nodeId && pkt.dest != 255)
    return;

  // HELLO → auto discovery
  if (pkt.type == ENP_MSG_HELLO)
  {
    if (pkt.src == instance->nodeId) return;

    bool found = false;

    for (int i = 0; i < instance->peerCount; i++)
    {
      if (instance->peers[i].id == pkt.src)
      {
        found = true;
        break;
      }
    }

    if (!found && instance->peerCount < ENP_MAX_PEERS)
    {
      instance->peers[instance->peerCount].id = pkt.src;
      memcpy(instance->peers[instance->peerCount].mac, info->src_addr, 6);

      esp_now_peer_info_t peerInfo = {};
      memcpy(peerInfo.peer_addr, info->src_addr, 6);
      esp_now_add_peer(&peerInfo);

      instance->peerCount++;

      Serial.print("[ENP] New peer discovered: ");
      Serial.println(pkt.src);
    }
  }

  // ACK
  if (pkt.type == ENP_MSG_DATA)
  {
    enp_packet_t ack = {};

    ack.type = ENP_MSG_ACK;
    ack.src  = instance->nodeId;
    ack.dest = pkt.src;
    ack.seq  = pkt.seq;

    esp_now_send(info->src_addr, (uint8_t *)&ack, sizeof(ack));
  }

  // ACK recebido
  if (pkt.type == ENP_MSG_ACK)
  {
    if (instance->waitingAck)
    {
      if (pkt.seq == instance->pendingSeq)
      {
        instance->waitingAck = false;
      }
    }
  }

  // callback
  if (pkt.type == ENP_MSG_DATA)
  {
    if (instance->receiveCallback)
    {
      instance->receiveCallback(pkt.src, pkt.id, pkt.payload, pkt.len);
    }
  }
}