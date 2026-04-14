#pragma once

#include <Arduino.h>

#define ENP_MSG_DATA    1
#define ENP_MSG_ACK     2

#define ENP_MAX_PAYLOAD 32

typedef struct {
  uint8_t type;
  uint8_t id;
  uint8_t len;
  uint8_t payload[ENP_MAX_PAYLOAD];
  uint8_t seq;
} enp_packet_t;