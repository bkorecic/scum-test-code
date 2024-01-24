#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bch.h"

char _msg[] = "holbholahola";
char _msg_corrupt[] = "holakkkkkkkk";

int main(){

	struct bch_control *bch = bch_init(15, 7, 0, false);
	
	uint8_t *msg = _msg;
	uint8_t *msg_corrupt = _msg_corrupt;

	uint8_t *ecc = malloc(bch->ecc_bytes*sizeof(uint8_t));
	memset(ecc, 0, bch->ecc_bytes*sizeof(uint8_t));

	bch_encode(bch, msg, 12, ecc);

	uint32_t *errloc = malloc((12+bch->ecc_bytes)*sizeof(uint8_t));

	int n_errors = bch_decode(bch, msg_corrupt, 12, ecc, NULL, NULL, errloc);
	
	printf("Decoded message with n_errors: %d\n", n_errors);

	for(int i=0; i<n_errors; i++){
		printf("Error number %d in position %d\n", i, errloc[i]);
	}

	bch_free(bch);

	return 0;
}
