#pragma once

#include <Arduino.h>

// Tipos de mensagem
#define ENP_MSG_CMD     1
#define ENP_MSG_ACK     2
#define ENP_MSG_STATUS  4

// Comandos básicos
#define ENP_CMD_STOP     0
#define ENP_CMD_FORWARD  1
#define ENP_CMD_BACKWARD 2
#define ENP_CMD_LEFT     3
#define ENP_CMD_RIGHT    4

// Estrutura padrão
typedef struct {
  uint8_t type;
  uint8_t command;
  int32_t value;
  uint8_t seq;
} enp_message_t;