#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

// --------------------------------------------------

uint8_t nodeId;

// --------------------------------------------------

void onData(uint8_t src, uint8_t id, const uint8_t *data, uint8_t len)
{
  char buffer[33] = {0};
  memcpy(buffer, data, len);

  Serial.print("[RX] From ");
  Serial.print(src);
  Serial.print(" | Msg: ");
  Serial.println(buffer);
}

// --------------------------------------------------

void setup()
{
  Serial.begin(115200);
  delay(1000);

  // Gera um ID único simples
  nodeId = random(1, 200);

  Serial.println("=================================");
  Serial.println(" ESPNowProtocol Auto Discovery");
  Serial.print(" Node ID: ");
  Serial.println(nodeId);
  Serial.println("=================================");

  protocol.begin();
  protocol.setNodeId(nodeId);

  protocol.enableAutoDiscovery(true);
  protocol.onReceive(onData);
}

// --------------------------------------------------

unsigned long lastSend = 0;
unsigned long lastPrint = 0;

void loop()
{
  protocol.loop();

  // envia mensagem periódica (broadcast)
  if (millis() - lastSend > 3000)
  {
    const char *msg = "Hello network";

    Serial.println("[TX] Broadcast message");
    protocol.sendReliable(255, 1, (uint8_t*)msg, strlen(msg));

    lastSend = millis();
  }

  // debug simples de tempo (mostra que está vivo)
  if (millis() - lastPrint > 2000)
  {
    Serial.println("[INFO] Running...");
    lastPrint = millis();
  }
}