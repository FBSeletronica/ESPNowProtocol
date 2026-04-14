#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

unsigned long lastStatus = 0;

void handleCommand(uint8_t cmd, int32_t value)
{
  Serial.print("CMD: ");
  Serial.println(cmd);
}

void setup()
{
  Serial.begin(115200);

  protocol.begin();
  protocol.onCommand(handleCommand);

  Serial.println("Robot ready");
}

void loop()
{
  protocol.loop();

  if (millis() - lastStatus > 1000)
  {
    int fakeRSSI = random(-90, -30);

    protocol.sendStatus(fakeRSSI);

    lastStatus = millis();
  }
}