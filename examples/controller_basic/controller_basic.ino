#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

void setup()
{
  Serial.begin(115200);
  protocol.begin();
}

void loop()
{
  protocol.sendCommand(1, 100);
  delay(1000);
}