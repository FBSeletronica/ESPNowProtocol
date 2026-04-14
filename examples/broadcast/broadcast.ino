/*
 * Example: broadcast
 *
 * Sends the same text payload to every nearby node using broadcast address
 * 255. The registered peers are still useful for direct messaging later.
 */

#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

// Example peers that may also receive direct messages in other scenarios.
uint8_t peer1[] = {0x10,0x52,0x1C,0x68,0x15,0x28};
uint8_t peer2[] = {0x24,0x6F,0x28,0xAA,0xBB,0xCC};

void setup()
{
  Serial.begin(115200);

  // Initialize the protocol stack and assign this device a logical id.
  protocol.begin();
  protocol.setNodeId(1);

  // Register known peers for future unicast communication.
  protocol.addPeer(2, peer1);
  protocol.addPeer(3, peer2);
}

void loop()
{
  // Keep the protocol state machine running.
  protocol.loop();

  const char *msg = "Broadcast";

  // Destination 255 means "send to all nodes".
  Serial.println("Broadcasting...");
  protocol.sendReliable(255, 1, (uint8_t*)msg, strlen(msg));

  delay(3000);
}
