#include "ESPNowProtocol.h"

static ESPNowProtocol* instance = nullptr;

// --------------------------------------------------

bool ESPNowProtocol::isQueueEmpty()
{
  return (!queueFull && (queueHead == queueTail));
}

bool ESPNowProtocol::isQueueFull()
{
  return queueFull;
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

void ESPNowProtocol::begin()
{
  instance = this;

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("[ENP] init failed");
    return;
  }

  esp_now_register_recv_cb(ESPNowProtocol::onReceiveInternal);
}

// --------------------------------------------------

void ESPNowProtocol::loop()
{
  // envia próximo pacote da fila
  if (!waitingAck)
  {
    enp_packet_t pkt;

    if (popQueue(pkt))
    {
      lastPacket = pkt;

      esp_now_send(peerAddress, (uint8_t *)&lastPacket, sizeof(lastPacket));

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

        esp_now_send(peerAddress, (uint8_t *)&lastPacket, sizeof(lastPacket));
        ackStartTime = millis();
      }
      else
      {
        waitingAck = false;
      }
    }
  }
}

// --------------------------------------------------

void ESPNowProtocol::setPeer(const uint8_t *mac)
{
  memcpy(peerAddress, mac, 6);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, mac, 6);

  if (!esp_now_is_peer_exist(mac))
  {
    esp_now_add_peer(&peerInfo);
  }
}

// --------------------------------------------------

void ESPNowProtocol::send(uint8_t id, const uint8_t *data, uint8_t len)
{
  if (len > ENP_MAX_PAYLOAD) return;

  enp_packet_t pkt = {};

  pkt.type = ENP_MSG_DATA;
  pkt.id = id;
  pkt.len = len;
  memcpy(pkt.payload, data, len);
  pkt.seq = seqCounter++;

  esp_now_send(peerAddress, (uint8_t *)&pkt, sizeof(pkt));
}

// --------------------------------------------------

void ESPNowProtocol::sendReliable(uint8_t id, const uint8_t *data, uint8_t len)
{
  if (len > ENP_MAX_PAYLOAD) return;

  enp_packet_t pkt = {};

  pkt.type = ENP_MSG_DATA;
  pkt.id = id;
  pkt.len = len;
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

  if (instance)
  {
    memcpy(instance->peerAddress, info->src_addr, 6);

    if (!esp_now_is_peer_exist(info->src_addr))
    {
      esp_now_peer_info_t peerInfo = {};
      memcpy(peerInfo.peer_addr, info->src_addr, 6);
      esp_now_add_peer(&peerInfo);
    }
  }

  enp_packet_t pkt;
  memcpy(&pkt, data, sizeof(pkt));

  // ACK automático
  if (pkt.type == ENP_MSG_DATA)
  {
    enp_packet_t ack = {};

    ack.type = ENP_MSG_ACK;
    ack.id = 0;
    ack.len = 0;
    ack.seq = pkt.seq;

    esp_now_send(info->src_addr, (uint8_t *)&ack, sizeof(ack));
  }

  // Recebe ACK
  if (pkt.type == ENP_MSG_ACK)
  {
    if (instance && instance->waitingAck)
    {
      if (pkt.seq == instance->pendingSeq)
      {
        instance->waitingAck = false;
      }
    }
  }

  // Callback de dados
  if (pkt.type == ENP_MSG_DATA)
  {
    if (instance && instance->receiveCallback)
    {
      instance->receiveCallback(pkt.id, pkt.payload, pkt.len);
    }
  }
}