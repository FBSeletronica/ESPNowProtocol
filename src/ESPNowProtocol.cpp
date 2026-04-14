#include "ESPNowProtocol.h"

static ESPNowProtocol* instance = nullptr;

void ESPNowProtocol::begin()
{
  instance = this;

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("[ENP] ESP-NOW init failed");
    return;
  }

  esp_now_register_recv_cb(ESPNowProtocol::onReceive);

  Serial.println("[ENP] started");
}

void ESPNowProtocol::loop()
{
}

void ESPNowProtocol::setPeer(const uint8_t *mac)
{
  memcpy(peerAddress, mac, 6);

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, mac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (!esp_now_is_peer_exist(mac))
  {
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
      Serial.println("[ENP] peer add failed");
      return;
    }
  }
}

void ESPNowProtocol::sendCommand(uint8_t cmd, int32_t value)
{
  enp_message_t msg;

  msg.type = ENP_MSG_CMD;
  msg.command = cmd;
  msg.value = value;
  msg.seq = seqCounter++;

  esp_now_send(peerAddress, (uint8_t *)&msg, sizeof(msg));
}

void ESPNowProtocol::onCommand(enp_command_cb_t cb)
{
  commandCallback = cb;
}

void ESPNowProtocol::onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len)
{
  if (len < sizeof(enp_message_t))
    return;

  enp_message_t msg;
  memcpy(&msg, data, sizeof(msg));

  if (instance && instance->commandCallback)
  {
    if (msg.type == ENP_MSG_CMD)
    {
      instance->commandCallback(msg.command, msg.value);
    }
  }
}