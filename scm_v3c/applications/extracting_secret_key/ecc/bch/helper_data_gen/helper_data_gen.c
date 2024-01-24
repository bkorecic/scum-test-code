#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "bch.h"
#include "memory_map.h"
#include "optical.h"
#include "params.h"
#include "scm3c_hw_interface.h"

//=========================== defines =========================================

#define CRC_VALUE (*((unsigned int*)0x0000FFFC))
#define CODE_LENGTH (*((unsigned int*)0x0000FFF8))

//=========================== variables =======================================

typedef struct {
    uint8_t sram_data[KEY_LEN / BYTE_SIZE];  // raw data from the SRAM
} app_vars_t;

app_vars_t app_vars;

//=========================== prototypes ======================================

//=========================== main ============================================

int main(void) {
    uint32_t i;
    struct bch_control *bch;
    uint8_t *ecc;

    printf("Initializing...");

    initialize_mote();
    crc_check();
    perform_calibration();

    // read bytes
    printf("Reading sram data\n");
    for (i = 0; i < KEY_LEN / BYTE_SIZE; i++) {
        app_vars.sram_data[i] = *(PUF_ADDR + i);
    }

    // initialize struct for bch
    bch = bch_init(15, 7, 0, false);

    // initialize ecc bytearray
    ecc = malloc(bch->ecc_bytes * sizeof(uint8_t));
    memset(ecc, 0, bch->ecc_bytes * sizeof(uint8_t));

    // generate ECC helper data
    bch_encode(bch, app_vars.sram_data, KEY_LEN / BYTE_SIZE, ecc);

    // print the helper data
    printf("Helper data: \"");
    for (i = 0; i < bch->ecc_bytes; i++) {
        printf("%02x", ecc[i]);
    }
    printf("\"\n");
    return 0;
}

//=========================== public ==========================================

//=========================== private =========================================
