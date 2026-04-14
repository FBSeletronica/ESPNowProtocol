#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

uint8_t robotMAC[] = {0x10,0x52,0x1C,0x68,0x15,0x28};

void setup()
{
  Serial.begin(115200);

  protocol.begin();
  protocol.setPeer(robotMAC);

  Serial.println("Controller ready");
}

void loop()
{
  protocol.loop();

  protocol.sendReliable(ENP_CMD_FORWARD, 150);
  delay(2000);

  protocol.sendReliable(ENP_CMD_STOP, 0);
  delay(2000);
}