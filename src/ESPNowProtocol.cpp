#include "ESPNowProtocol.h"

// --------------------------------------------------
// Instância global (necessária para callback estático)

static ESPNowProtocol* instance = nullptr;

// --------------------------------------------------

void ESPNowProtocol::begin()
{
  instance = this;

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("[ENP] Erro ao iniciar ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(ESPNowProtocol::onReceive);

  Serial.println("[ENP] ESPNowProtocol iniciado");
}

// --------------------------------------------------

void ESPNowProtocol::loop()
{
  // v0.2 → ainda não há lógica
  // futuro:
  // - retry
  // - timeout
  // - status
}

// --------------------------------------------------

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
      Serial.println("[ENP] Erro ao adicionar peer");
      return;
    }
  }

  Serial.print("[ENP] Peer configurado: ");
  for (int i = 0; i < 6; i++)
  {
    Serial.print(peerAddress[i], HEX);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
}

// --------------------------------------------------

void ESPNowProtocol::sendCommand(uint8_t cmd, int32_t value)
{
  enp_message_t msg;

  msg.type = ENP_MSG_CMD;
  msg.command = cmd;
  msg.value = value;
  msg.seq = seqCounter++;

  esp_err_t result = esp_now_send(peerAddress, (uint8_t *)&msg, sizeof(msg));

  Serial.print("[ENP TX] CMD: ");
  Serial.print(cmd);
  Serial.print(" | VAL: ");
  Serial.print(value);
  Serial.print(" | SEQ: ");
  Serial.print(msg.seq);

  if (result == ESP_OK)
    Serial.println(" | OK");
  else
    Serial.println(" | ERRO");
}

// --------------------------------------------------
// CALLBACK RECEPÇÃO

void ESPNowProtocol::onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len)
{
  if (len < sizeof(enp_message_t))
    return;

  enp_message_t msg;
  memcpy(&msg, data, sizeof(msg));

  Serial.print("[ENP RX] TYPE: ");
  Serial.print(msg.type);
  Serial.print(" | CMD: ");
  Serial.print(msg.command);
  Serial.print(" | VAL: ");
  Serial.print(msg.value);
  Serial.print(" | SEQ: ");
  Serial.println(msg.seq);

  // v0.2 → apenas debug
  // v0.3 → callback real (onCommand)
}