#include <stdio.h>
#include <stdint.h>

#include "memory_map.h"
#include "optical.h"
#include "scm3c_hw_interface.h"

//=========================== defines =========================================

#define CRC_VALUE (*((unsigned int*)0x0000FFFC))
#define CODE_LENGTH (*((unsigned int*)0x0000FFF8))

#define PUF_ADDR ((uint8_t*)0x20005000) // adequate SCuM SRAM address for PUF
#define BYTE_SIZE 8                     //
#define KEY_LEN 64                      // length of key
#define REP_FACTOR 9                    // repetition factor of ecc, must be odd
#define READ_BITS (KEY_LEN*REP_FACTOR)  // number of bits to read from the SRAM

//=========================== variables =======================================

const uint8_t ecc_hd[] = // helper data for ECC
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

typedef struct {
    uint8_t sram_data[READ_BITS/BYTE_SIZE];   // raw data from the SRAM
    uint8_t key[KEY_LEN/BYTE_SIZE];
} app_vars_t;

app_vars_t app_vars;

//=========================== prototypes ======================================

uint8_t get_bit(uint8_t* byte_arr, uint32_t pos);

void set_bit(uint8_t* byte_arr, uint32_t pos, uint8_t val);

//=========================== main ============================================

int main(void) {
    uint32_t i, j, count;
    uint8_t data_bit;

    printf("Initializing...");

    initialize_mote();
    crc_check();
    perform_calibration();

    // read bytes
    for(i=0; i<READ_BITS/BYTE_SIZE; i++){
        // calculate XOR with helper data as we read
        app_vars.sram_data[i] = ecc_hd[i]^(*(PUF_ADDR+i));
    }

    // recover the key
    for(i=0; i<KEY_LEN; i++){
        // i iterates through each segment (of size REP_FACTOR bits)
        count = 0; // count of 1-bits in segment
        for(j=0; j<REP_FACTOR; j++){
            // j iterates through each bit in the segment to count
            data_bit = get_bit(app_vars.sram_data, REP_FACTOR * i + j);
            if(data_bit) count++;
        }
        if(count > REP_FACTOR - count){
            set_bit(app_vars.key, i, 1);
        }
        else{
            set_bit(app_vars.key, i, 0);
        }
    }

    // print the key
    printf("Key: \"");
    for(i=0; i<KEY_LEN/BYTE_SIZE; i++){
        printf("%02x", app_vars.key[i]);
    }
    printf("\"\n");
    return 0;
}

//=========================== public ==========================================

//=========================== private =========================================

// utility function to get the bit indexed at "pos" of
// bytearray "byte_arr"
uint8_t get_bit(uint8_t* byte_arr, uint32_t pos){
    return (byte_arr[pos/BYTE_SIZE]>>(pos%BYTE_SIZE))&1;
}

// utility function to set the bit indexed at "pos" to
// "val" in the bytearray "byte_arr"
void set_bit(uint8_t* byte_arr, uint32_t pos, uint8_t val){
    if(val){
        byte_arr[pos/BYTE_SIZE] |= (1 << (pos%BYTE_SIZE));
    }
    else {
        byte_arr[pos/BYTE_SIZE] &= ~(1 << (pos&BYTE_SIZE));
    }
}
