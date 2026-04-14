#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

// MAC address of the receiver (replace with your receiver's MAC address)
uint8_t peer[] = {0x10,0x52,0x1C,0x68,0x15,0x28};

int step = 0;
unsigned long lastSend = 0;

void setup()
{
  Serial.begin(115200);

  protocol.begin();
  protocol.setPeer(peer);

  protocol.setNodeId(1); // sender ID

  Serial.println("Sender ready");
}

void loop()
{
  protocol.loop();

  if (millis() - lastSend > 1000)
  {
    if (step == 0)
    {
      int value = 42;
      protocol.sendReliable(2, 1, (uint8_t*)&value, sizeof(value));
    }
    else if (step == 1)
    {
      float f = 3.14;
      protocol.sendReliable(2, 2, (uint8_t*)&f, sizeof(f));
    }
    else if (step == 2)
    {
      const char *msg = "Hello ESP-NOW";
      protocol.sendReliable(2, 3, (uint8_t*)msg, strlen(msg));
    }

    step = (step + 1) % 3;
    lastSend = millis();
  }
}