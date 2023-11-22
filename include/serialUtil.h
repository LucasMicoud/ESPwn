#ifndef SERIAL_UTIL
#define SERIAL_UTIL

#include <Arduino.h>
#include "inttypes.h"

#define SNAPLEN 65535
#define FCS_LENGTH 4 // Length of frame check sequence.

void serialout_32bit(uint32_t input);

void serialout_16bit(uint16_t input);

void serialPacket(uint32_t len, uint8_t* payload_buf);

#endif // SERIAL_UTIL