# SRAM PUF with ECC

## Repetition code

This directory contains the implementation of an SRAM PUF using repetition code for error correcting.

* repetition_code.{c,h} contains parameters for the ECC,
* bit_utils.{c,h} contains utility functions to access and modify bits in a byte array,
* helper_data_gen/ contains the application for generating the ECC helper data, and
* read_key/ contains the application for extracting a secret key and decoding it according to the ECC.
