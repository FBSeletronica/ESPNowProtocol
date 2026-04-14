#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

void onData(uint8_t src, uint8_t id, const uint8_t *data, uint8_t len)
{
  int value;
  memcpy(&value, data, sizeof(value));

  Serial.print("From ");
  Serial.print(src);
  Serial.print(": ");
  Serial.println(value);
}

void setup()
{
  Serial.begin(115200);

  protocol.begin();
  protocol.setNodeId(2);

  protocol.onReceive(onData);
}

void loop()
{
  protocol.loop();
}