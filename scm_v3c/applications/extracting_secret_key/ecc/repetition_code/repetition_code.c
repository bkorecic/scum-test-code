#include "repetition_code.h"

const uint8_t hard_coded_ecc_hd[] = // helper data for ECC
{
    0x1a, 0x89, 0x42, 0xd3, 0x4c, 0xdc,
    0x84, 0x1d, 0xea, 0x98, 0x64, 0xf0,
    0xe6, 0x27, 0xc0, 0x33, 0x6a, 0xd2,
    0xaa, 0xb8, 0x72, 0x62, 0x2a, 0x45,
    0x90, 0x77, 0xf4, 0x22, 0xc8, 0xb1,
    0x50, 0x04, 0x56, 0x8a, 0x36, 0x40,
    0xe4, 0xb1, 0x42, 0x90, 0xc4, 0xda,
    0x06, 0x6d, 0xb8, 0x6a, 0x3c, 0xd1,
    0x70, 0xe9, 0xc8, 0x35, 0x3e, 0xc2,
    0x7a, 0x48, 0x0a, 0x61, 0xec, 0xd7,
    0x3a, 0x15, 0xdc, 0xee, 0xd0, 0xd1,
    0x90, 0x8d, 0x9c, 0x94, 0x68, 0xe4};
