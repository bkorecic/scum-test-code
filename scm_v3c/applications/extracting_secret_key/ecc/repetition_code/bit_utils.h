#ifndef __BIT_UTILS_H
#define __BIT_UTILS_H

#include <stdint.h>

#define BYTE_SIZE 8

uint8_t get_bit(uint8_t* byte_arr, uint32_t pos);

void set_bit(uint8_t* byte_arr, uint32_t pos, uint8_t val);

#endif
