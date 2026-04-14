#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

void onData(uint8_t src, uint8_t id, const uint8_t *data, uint8_t len)
{
  char buffer[33] = {0};
  memcpy(buffer, data, len);

  Serial.print("From ");
  Serial.print(src);
  Serial.print(": ");
  Serial.println(buffer);
}

void setup()
{
  Serial.begin(115200);

  protocol.begin();
  protocol.setNodeId(random(1, 200));

  protocol.enableAutoDiscovery(true);
  protocol.onReceive(onData);

  Serial.println("Auto discovery enabled");
}

void loop()
{
  protocol.loop();

  static unsigned long last = 0;

  if (millis() - last > 3000)
  {
    const char *msg = "Hello network";

    protocol.sendReliable(255, 1, (uint8_t*)msg, strlen(msg));

    last = millis();
  }
}