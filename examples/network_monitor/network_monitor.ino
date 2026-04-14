#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

uint8_t nodeId;

// --------------------------------------------------

void onData(uint8_t src, uint8_t id, const uint8_t *data, uint8_t len, int8_t rssi)
{
  Serial.print("[RX] From ");
  Serial.print(src);

  Serial.print(" | RSSI: ");
  Serial.print(rssi);

  Serial.print(" dBm | Quality: ");
  Serial.print(protocol.getLinkQuality(src));
  Serial.println("%");
}

// --------------------------------------------------

void setup()
{
  Serial.begin(115200);
  delay(1000);

  nodeId = esp_random() % 200;

  Serial.println("=================================");
  Serial.println(" ESPNowProtocol Network Monitor");
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

  // envia broadcast periódico
  if (millis() - lastSend > 3000)
  {
    const char *msg = "ping";

    protocol.sendReliable(255, 1, (uint8_t*)msg, strlen(msg));

    lastSend = millis();
  }

  // imprime estado da rede
  if (millis() - lastPrint > 2000)
  {
    Serial.println("\n--- Network Status ---");

    for (int i = 0; i < 10; i++)
    {
      int8_t rssi = protocol.getPeerRSSI(i);
      uint8_t quality = protocol.getLinkQuality(i);

      if (rssi != 0)
      {
        Serial.print("Peer ");
        Serial.print(i);

        Serial.print(" | RSSI: ");
        Serial.print(rssi);

        Serial.print(" | Quality: ");
        Serial.print(quality);
        Serial.println("%");
      }
    }

    Serial.println("----------------------");

    lastPrint = millis();
  }
}