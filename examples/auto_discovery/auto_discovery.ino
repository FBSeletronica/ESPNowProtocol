/*
 * Example: auto_discovery
 *
 * Starts a node with a random logical id, enables automatic discovery, and
 * periodically broadcasts a text message so nearby nodes can detect it.
 */

#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

// --------------------------------------------------

uint8_t nodeId;

// --------------------------------------------------

void onData(uint8_t src, uint8_t id, const uint8_t *data, uint8_t len, int8_t rssi)
{
  char buffer[33] = {0};
  memcpy(buffer, data, len);

  Serial.print("[RX] From ");
  Serial.print(src);
  Serial.print(" | Msg ID: ");
  Serial.print(id);
  Serial.print(" | RSSI: ");
  Serial.print(rssi);
  Serial.print(" dBm");
  Serial.print(" | Msg: ");
  Serial.println(buffer);
}

// --------------------------------------------------

void setup()
{
  Serial.begin(115200);
  delay(1000);

  // Generate a simple random logical node id for demo purposes.
  nodeId = random(1, 200);

  Serial.println("=================================");
  Serial.println(" ESPNowProtocol Auto Discovery");
  Serial.print(" Node ID: ");
  Serial.println(nodeId);
  Serial.println("=================================");

  // Start the protocol, enable discovery, and listen for payload packets.
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
  // Keep the protocol state machine running.
  protocol.loop();

  // Broadcast a message periodically so nearby nodes can discover this device.
  if (millis() - lastSend > 3000)
  {
    const char *msg = "Hello network";

    Serial.println("[TX] Broadcast message");
    protocol.sendReliable(255, 1, (uint8_t*)msg, strlen(msg));

    lastSend = millis();
  }

  // Print a small heartbeat to show the sketch is still active.
  if (millis() - lastPrint > 2000)
  {
    Serial.println("[INFO] Running...");
    lastPrint = millis();
  }
}
