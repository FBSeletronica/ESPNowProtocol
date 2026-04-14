#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include "ESPNowTypes.h"

// ==================================================
// ESPNowProtocol v0.2
// - Inicialização ESP-NOW
// - Envio de comandos
// - Recepção básica (debug)
// - Estrutura pronta para evolução (callbacks, ACK, etc.)
// ==================================================

class ESPNowProtocol {
public:

  // --------------------------------------------------
  // Inicialização

  void begin();

  // --------------------------------------------------
  // Loop interno (engine da biblioteca)

  void loop();

  // --------------------------------------------------
  // Configuração de peer (destino)

  void setPeer(const uint8_t *mac);

  // --------------------------------------------------
  // Envio de comando

  void sendCommand(uint8_t cmd, int32_t value);

private:

  // --------------------------------------------------
  // Callback interno de recepção (ESP-NOW)

  static void onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len);

  // --------------------------------------------------
  // Estado interno

  uint8_t peerAddress[6] = {0};
  uint8_t seqCounter = 0;
};