#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "memory_map.h"
#include "optical.h"
#include "scm3c_hw_interface.h"
#include "bch.h"
#include "params.h"
#include "bit_utils.h"

//=========================== defines =========================================

#define CRC_VALUE (*((unsigned int*)0x0000FFFC))
#define CODE_LENGTH (*((unsigned int*)0x0000FFF8))

//=========================== variables =======================================


typedef struct {
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
    for(i=0; i<KEY_LEN/BYTE_SIZE; i++){
        // calculate XOR with helper data as we read
        app_vars.key[i] = *(PUF_ADDR+i);
    }

    struct bch_control *bch = bch_init(15, 7, 0, false); // TODO: use params.h
    uint8_t *ecc; // TODO: implement in params.h

    // find errors using bch decoder
    uint32_t *errloc = malloc(42*sizeof(uint8_t)); // TODO: KEY_LEN+ecc_bytes
    
    int n_errors = bch_decode(bch, app_vars.key, KEY_LEN, ecc, NULL, NULL, errloc);
    
    if(n_errors < 0){
        printf("Error in decoding\n");
        return n_errors;
    }
    
    printf("Decoded message with n_errors: \"%d\"\n", n_errors);
    
    // flip error bits to recover key
    for(i=0; i<n_errors; i++){
        if(errloc[i] < BYTE_SIZE*KEY_LEN) {
            uint8_t bit = get_bit(app_vars.key, errloc[i]);
            bit ^= 1;
            set_bit(app_vars.key, errloc[i], bit);
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
