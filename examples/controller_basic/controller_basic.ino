#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

uint8_t robotMAC[] = {0x10,0x52,0x1C,0x68,0x15,0x28};

void setup()
{
  Serial.begin(115200);

  protocol.begin();
  protocol.setPeer(robotMAC);
}

void loop()
{
  protocol.loop();

  protocol.sendCommand(ENP_CMD_FORWARD, 100);
  delay(2000);

  protocol.sendCommand(ENP_CMD_STOP, 0);
  delay(2000);
}