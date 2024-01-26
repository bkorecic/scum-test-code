/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Generic binary BCH encoding/decoding library
 *
 * Copyright © 2011 Parrot S.A.
 *
 * Author: Ivan Djelic <ivan.djelic@parrot.com>
 *
 * Description:
 *
 * This library provides runtime configurable encoding/decoding of binary
 * Bose-Chaudhuri-Hocquenghem (BCH) codes.
*/
#ifndef _BCH_H
#define _BCH_H

#include <stdint.h>
#include <stdbool.h>
#include "params.h"

#define GF_M		(CONFIG_BCH_CONST_M)
#define GF_T		(CONFIG_BCH_CONST_T)
#define GF_N		((1 << (CONFIG_BCH_CONST_M))-1)
#define BCH_MAX_M	(CONFIG_BCH_CONST_M)
#define BCH_MAX_T	(CONFIG_BCH_CONST_T)

#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

#define BCH_ECC_WORDS      DIV_ROUND_UP(GF_M*GF_T, 32)
#define BCH_ECC_BYTES      DIV_ROUND_UP(GF_M*GF_T, 8)

#define BCH_ECC_MAX_WORDS      DIV_ROUND_UP(BCH_MAX_M * BCH_MAX_T, 32)

/*
 * represent a polynomial over GF(2^m)
 */
struct gf_poly {
	unsigned int deg;    /* polynomial degree */
	unsigned int c[GF_M * GF_T];   /* polynomial terms */
};

/* given its degree, compute a polynomial size in bytes */
#define GF_POLY_SZ(_d) (sizeof(struct gf_poly)+((_d)+1)*sizeof(unsigned int))

/* polynomial of degree 1 */
struct gf_poly_deg1 {
	struct gf_poly poly;
	unsigned int   c[2];
};

/**
 * struct bch_control - BCH control structure
 * @m:          Galois field order
 * @n:          maximum codeword size in bits (= 2^m-1)
 * @t:          error correction capability in bits
 * @ecc_bits:   ecc exact size in bits, i.e. generator polynomial degree (<=m*t)
 * @ecc_bytes:  ecc max size (m*t bits) in bytes
 * @a_pow_tab:  Galois field GF(2^m) exponentiation lookup table
 * @a_log_tab:  Galois field GF(2^m) log lookup table
 * @mod8_tab:   remainder generator polynomial lookup tables
 * @ecc_buf:    ecc parity words buffer
 * @ecc_buf2:   ecc parity words buffer
 * @xi_tab:     GF(2^m) base for solving degree 2 polynomial roots
 * @syn:        syndrome buffer
 * @cache:      log-based polynomial representation buffer
 * @elp:        error locator polynomial
 * @poly_2t:    temporary polynomials of degree 2t
 * @swap_bits:  swap bits within data and syndrome bytes
 */
struct bch_control {
	unsigned int    m;
	unsigned int    n;
	unsigned int    t;
	unsigned int    ecc_bits;
	unsigned int    ecc_bytes;
/* private: */
	uint16_t        a_pow_tab[1 + GF_N];
	uint16_t        a_log_tab[1 + GF_N];
	uint32_t        mod8_tab[1024 * BCH_ECC_WORDS];
	uint32_t        ecc_buf[BCH_ECC_WORDS];
	uint32_t        ecc_buf2[BCH_ECC_WORDS];
	unsigned int    xi_tab[GF_M];
	unsigned int    syn[2 * GF_T];
	int             cache[2 * GF_T];
	struct gf_poly  elp;
	struct gf_poly  poly_2t[4];
	bool		swap_bits;
};

int bch_init(struct bch_control *bch, unsigned int prim_poly,
			     bool swap_bits);

void bch_encode(struct bch_control *bch, const uint8_t *data,
		unsigned int len, uint8_t *ecc);

int bch_decode(struct bch_control *bch, const uint8_t *data, unsigned int len,
	       const uint8_t *recv_ecc, const uint8_t *calc_ecc,
	       const unsigned int *syn, unsigned int *errloc);

#endif /* _BCH_H */
