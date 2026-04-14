/*
 * Example: basic_sender
 *
 * Sends an integer value from node 1 to node 2 using reliable delivery.
 * Update the peer MAC address below so it matches the receiver device.
 */

#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

// Replace this MAC address with the receiver's ESP-NOW MAC address.
uint8_t peer[] = {0x10,0x52,0x1C,0x68,0x15,0x28};

void setup()
{
  Serial.begin(115200);

  // Initialize the protocol stack and assign this device a logical id.
  protocol.begin();
  protocol.setNodeId(1);

  // Register node 2 as a known peer so unicast traffic can be sent to it.
  protocol.addPeer(2, peer);
}

void loop()
{
  // Keep the protocol state machine running.
  protocol.loop();

  int value = 42;

  // Queue a reliable message addressed to node 2.
  protocol.sendReliable(2, 1, (uint8_t*)&value, sizeof(value));

  delay(1000);
}
