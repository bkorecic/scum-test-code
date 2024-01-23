#ifndef __REPETITION_CODE_H
#define __REPETITION_CODE_H

/*
* This file contains parameters for the repetition code ECC SRAM PUF.
*/

#include <stdint.h>

#define PUF_ADDR ((uint8_t*)0x20005000) // adequate SCuM SRAM address for PUF
#define KEY_LEN 64                      // length of key
#define REP_FACTOR 9                    // repetition factor of ecc, must be odd
#define READ_BITS (KEY_LEN*REP_FACTOR)  // number of bits to read from the SRAM

// hd stands for "helper data". It is generated with the helper_data_gen application, hard-coded into this variable
// and then used in the "read_key" application.
extern const uint8_t hard_coded_ecc_hd[];

#endif
