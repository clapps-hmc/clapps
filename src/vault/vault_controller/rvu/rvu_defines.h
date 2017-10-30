/*
 * rvu_defines.h
 *
 *  Created on: 4 de jul de 2017
 *      Author: root
 */

#define SCALAR_WIDTH 32
#define NUM_SCALAR_REGS 64
#define NUM_VECT_REGS 8

#define LOAD32  	"1000000"
#define LOAD64 		"1000001"
#define LOAD128 	"1000010"
#define LOAD256 	"1000011"
#define STORE32 	"1010000"
#define STORE64 	"1010000"
#define STORE128 	"1010010"
#define STORE256 	"1010011"
#define ADD 		"0100000"
#define SUB 		"0100001"
#define OR 			"0100010"
#define AND 		"0100011"
#define XOR 		"0100100"
#define NOR 		"0100101"
#define NAND 		"0100110"
