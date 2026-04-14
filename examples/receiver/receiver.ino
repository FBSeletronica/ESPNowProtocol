#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

// --------------------------------------------------
// Callback de recepção

void onData(uint8_t id, const uint8_t *data, uint8_t len)
{
  Serial.print("Received ID: ");
  Serial.print(id);
  Serial.print(" | LEN: ");
  Serial.println(len);

  // Exemplo 1: inteiro
  if (id == 1 && len == sizeof(int))
  {
    int value;
    memcpy(&value, data, sizeof(value));

    Serial.print("Integer: ");
    Serial.println(value);
  }

  // Exemplo 2: struct simples
  else if (id == 2 && len == sizeof(float))
  {
    float f;
    memcpy(&f, data, sizeof(f));

    Serial.print("Float: ");
    Serial.println(f);
  }

  // Exemplo 3: string
  else if (id == 3)
  {
    char buffer[33] = {0};
    memcpy(buffer, data, len);

    Serial.print("String: ");
    Serial.println(buffer);
  }

  else
  {
    Serial.println("Unknown payload");
  }
}

// --------------------------------------------------

void setup()
{
  Serial.begin(115200);

  protocol.begin();
  protocol.onReceive(onData);

  Serial.println("Receiver ready");
}

// --------------------------------------------------

void loop()
{
  protocol.loop();
}