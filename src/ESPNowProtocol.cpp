#include "ESPNowProtocol.h"

void ESPNowProtocol::begin()
{
  Serial.println("ESPNowProtocol iniciado");
}

void ESPNowProtocol::loop()
{
}

void ESPNowProtocol::sendCommand(uint8_t cmd, int32_t value)
{
  Serial.print("CMD: ");
  Serial.print(cmd);
  Serial.print(" | ");
  Serial.println(value);
}