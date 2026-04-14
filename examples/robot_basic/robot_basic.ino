#include <ESPNowProtocol.h>

ESPNowProtocol protocol;

void handleCommand(uint8_t cmd, int32_t value)
{
  Serial.print("CMD: ");
  Serial.print(cmd);
  Serial.print(" | VAL: ");
  Serial.println(value);

  switch (cmd)
  {
    case ENP_CMD_FORWARD:
      Serial.println("Forward");
      break;

    case ENP_CMD_BACKWARD:
      Serial.println("Backward");
      break;

    case ENP_CMD_LEFT:
      Serial.println("Left");
      break;

    case ENP_CMD_RIGHT:
      Serial.println("Right");
      break;

    case ENP_CMD_STOP:
      Serial.println("Stop");
      break;
  }
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
}