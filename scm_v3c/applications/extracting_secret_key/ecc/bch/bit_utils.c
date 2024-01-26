#include "bit_utils.h"

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
        byte_arr[pos/BYTE_SIZE] &= ~(1 << (pos%BYTE_SIZE));
    }
}
