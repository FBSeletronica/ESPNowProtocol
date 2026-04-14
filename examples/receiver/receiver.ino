#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

// --------------------------------------------------

void onData(uint8_t src, uint8_t id, const uint8_t *data, uint8_t len)
{
  Serial.print("From: ");
  Serial.print(src);
  Serial.print(" | ID: ");
  Serial.print(id);
  Serial.print(" | LEN: ");
  Serial.println(len);

  if (id == 1 && len == sizeof(int))
  {
    int value;
    memcpy(&value, data, sizeof(value));

    Serial.print("Integer: ");
    Serial.println(value);
  }
  else if (id == 2 && len == sizeof(float))
  {
    float f;
    memcpy(&f, data, sizeof(f));

    Serial.print("Float: ");
    Serial.println(f);
  }
  else if (id == 3)
  {
    char buffer[33] = {0};
    memcpy(buffer, data, len);

    Serial.print("String: ");
    Serial.println(buffer);
  }
}

// --------------------------------------------------

void setup()
{
  Serial.begin(115200);

  protocol.begin();
  protocol.setNodeId(2); // device ID

  protocol.onReceive(onData);

  Serial.println("Receiver ready");
}

// --------------------------------------------------

void loop()
{
  protocol.loop();
}