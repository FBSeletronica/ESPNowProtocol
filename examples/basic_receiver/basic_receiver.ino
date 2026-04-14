/*
 * Example: basic_receiver
 *
 * Receives integer payloads addressed to node 2 and prints them to the
 * Serial Monitor together with the sender id and RSSI value.
 */

#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

void onData(uint8_t src, uint8_t id, const uint8_t *data, uint8_t len, int8_t rssi)
{
  int value;
  memcpy(&value, data, sizeof(value));

  Serial.print("From ");
  Serial.print(src);
  Serial.print(" | Msg ID: ");
  Serial.print(id);
  Serial.print(" | RSSI: ");
  Serial.print(rssi);
  Serial.print(" dBm");
  Serial.print(": ");
  Serial.println(value);
}

void setup()
{
  Serial.begin(115200);

  // Initialize the protocol stack and assign this device a logical id.
  protocol.begin();
  protocol.setNodeId(2);

  // Register the callback that will receive incoming application payloads.
  protocol.onReceive(onData);
}

void loop()
{
  // Keep the protocol state machine running.
  protocol.loop();
}
