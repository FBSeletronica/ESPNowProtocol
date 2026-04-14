#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

// MAC address of the receiver devices (replace with your actual MAC addresses)
uint8_t device2[] = {0x10,0x52,0x1C,0x68,0x15,0x28};
uint8_t device3[] = {0x24,0x6F,0x28,0xAA,0xBB,0xCC};

int step = 0;
unsigned long lastSend = 0;

void setup()
{
  Serial.begin(115200);

  protocol.begin();
  protocol.setNodeId(1); // este dispositivo

  protocol.addPeer(2, device2);
  protocol.addPeer(3, device3);

  Serial.println("Sender ready");
}

void loop()
{
  protocol.loop();

  if (millis() - lastSend > 2000)
  {
    if (step == 0)
    {
      int value = 42;
      Serial.println("Send → Node 2 (int)");
      protocol.sendReliable(2, 1, (uint8_t*)&value, sizeof(value));
    }
    else if (step == 1)
    {
      float f = 3.14;
      Serial.println("Send → Node 3 (float)");
      protocol.sendReliable(3, 2, (uint8_t*)&f, sizeof(f));
    }
    else if (step == 2)
    {
      const char *msg = "Broadcast message";
      Serial.println("Broadcast → ALL");
      protocol.sendReliable(255, 3, (uint8_t*)msg, strlen(msg));
    }

    step = (step + 1) % 3;
    lastSend = millis();
  }
}