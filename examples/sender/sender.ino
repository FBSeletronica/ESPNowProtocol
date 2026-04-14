#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

uint8_t peer[] = {0x10,0x52,0x1C,0x68,0x15,0x28};

void setup()
{
  Serial.begin(115200);

  protocol.begin();
  protocol.setPeer(peer);
}

int step = 0;
unsigned long lastSend = 0;

void loop()
{
  protocol.loop();

  if (millis() - lastSend > 2000)
  {
    if (step == 0)
    {
      int value = 42;
      protocol.sendReliable(1, (uint8_t*)&value, sizeof(value));
    }
    else if (step == 1)
    {
      float f = 3.14;
      protocol.sendReliable(2, (uint8_t*)&f, sizeof(f));
    }
    else if (step == 2)
    {
      const char *msg = "Hello ESP-NOW";
      protocol.sendReliable(3, (uint8_t*)msg, strlen(msg));
    }

    step = (step + 1) % 3;
    lastSend = millis();
  }
}