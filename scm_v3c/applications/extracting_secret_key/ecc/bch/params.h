#ifndef __PARAMS_H
#define __PARAMS_H

#define CONFIG_BCH_CONST_PARAMS
#define CONFIG_BCH_CONST_M 7 // Galois field order
#define CONFIG_BCH_CONST_T 1 // Error correction capability in bits
#define PUF_ADDR ((uint8_t*)0x20005000) // adequate SCuM SRAM address for PUF
#define KEY_LEN 64
#define BYTE_SIZE 8

#endif
