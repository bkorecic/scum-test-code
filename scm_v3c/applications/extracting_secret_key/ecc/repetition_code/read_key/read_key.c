#include <stdio.h>
#include <stdint.h>

#include "memory_map.h"
#include "optical.h"
#include "scm3c_hw_interface.h"
#include "repetition_code.h"
#include "bit_utils.h"

//=========================== defines =========================================

#define CRC_VALUE (*((unsigned int*)0x0000FFFC))
#define CODE_LENGTH (*((unsigned int*)0x0000FFF8))

//=========================== variables =======================================


typedef struct {
    uint8_t sram_data[READ_BITS/BYTE_SIZE];   // raw data from the SRAM
    uint8_t key[KEY_LEN/BYTE_SIZE];
} app_vars_t;

app_vars_t app_vars;

//=========================== prototypes ======================================

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
        app_vars.sram_data[i] = hard_coded_ecc_hd[i]^(*(PUF_ADDR+i));
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
