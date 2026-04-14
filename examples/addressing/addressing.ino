/*
 * Example: addressing
 *
 * Demonstrates logical addressing by sending a random integer from node 1
 * directly to node 2. Replace the peer MAC address with the real target MAC.
 */

#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

// Replace this MAC address with the ESP-NOW MAC of node 2.
uint8_t peer[] = {0x10,0x52,0x1C,0x68,0x15,0x28};

void setup()
{
  Serial.begin(115200);

  // Initialize the protocol stack and assign this device a logical id.
  protocol.begin();
  protocol.setNodeId(1);

  // Register the destination peer used for unicast delivery.
  protocol.addPeer(2, peer);
}

void loop()
{
  // Keep the protocol state machine running.
  protocol.loop();

  int value = random(0, 100);

  // The logical destination is node 2, regardless of the physical MAC.
  Serial.println("Sending to node 2");
  protocol.sendReliable(2, 1, (uint8_t*)&value, sizeof(value));

  delay(2000);
}
