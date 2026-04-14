#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

uint8_t peer[] = {0x10,0x52,0x1C,0x68,0x15,0x28};

void setup()
{
  Serial.begin(115200);

  protocol.begin();
  protocol.setNodeId(1);

  protocol.addPeer(2, peer);
}

void loop()
{
  protocol.loop();

  int value = 42;
  protocol.sendReliable(2, 1, (uint8_t*)&value, sizeof(value));

  delay(1000);
}