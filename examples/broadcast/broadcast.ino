#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

uint8_t peer1[] = {0x10,0x52,0x1C,0x68,0x15,0x28};
uint8_t peer2[] = {0x24,0x6F,0x28,0xAA,0xBB,0xCC};

void setup()
{
  Serial.begin(115200);

  protocol.begin();
  protocol.setNodeId(1);

  protocol.addPeer(2, peer1);
  protocol.addPeer(3, peer2);
}

void loop()
{
  protocol.loop();

  const char *msg = "Broadcast";

  Serial.println("Broadcasting...");
  protocol.sendReliable(255, 1, (uint8_t*)msg, strlen(msg));

  delay(3000);
}