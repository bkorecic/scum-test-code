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

typedef struct {
    uint8_t sram_data[READ_BITS/BYTE_SIZE];   // raw data from the SRAM
    uint8_t ecc_hd[READ_BITS/BYTE_SIZE];    // generated ecc helper data
} app_vars_t;

app_vars_t app_vars;

//=========================== prototypes ======================================

uint8_t get_bit(uint8_t* byte_arr, uint32_t pos);

void set_bit(uint8_t* byte_arr, uint32_t pos, uint8_t val);

//=========================== main ============================================

int main(void) {
    uint32_t i, j;
    uint8_t nominal_bit, data_bit;

    printf("Initializing...");

    initialize_mote();
    crc_check();
    perform_calibration();

    // read bytes
    printf("Reading sram data\n");
    for(i=0; i<READ_BITS/BYTE_SIZE; i++){
        app_vars.sram_data[i] = *(PUF_ADDR+i);
    }
    
    // generate ECC helper data
    for(i=0; i<KEY_LEN; i++){
        // i iterates through each segment (of size REP_FACTOR bits)
        nominal_bit = get_bit(app_vars.sram_data, REP_FACTOR * i);
        for(j=0; j<REP_FACTOR; j++){
            // j iterates through each bit in the segment
            data_bit = get_bit(app_vars.sram_data, REP_FACTOR * i + j);
            // we compute the XOR with the nominal_bit to generate
            // a bit of helper data
            set_bit(app_vars.ecc_hd, REP_FACTOR * i + j, data_bit ^ nominal_bit);
        }
    }
    
    // print the helper data
    printf("Helper data: \"");
    for(i=0; i<READ_BITS/BYTE_SIZE; i++){
        printf("%02x", app_vars.ecc_hd[i]);
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
