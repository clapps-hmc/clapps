
#ifndef SRC_COMMON_DEFINES_H_
#define SRC_COMMON_DEFINES_H_

#define NUMBER_OF_VAULTS 32
#define NUMBER_OF_BANKS 16
#define LOG_NUMBER_OF_BANKS 4
#define NUMBER_OF_LINKS 4
#define VAULTS_PER_QUADRANT 8

#define MAX_BLOCK_SIZE 32 //IN BYTES
#define BANK_SIZE 1 //IN MBYTES
#define TSV_WIDTH 32 //IN BYTES


#define FLIT_SIZE 128 //IN BITS
#define MUX_SELECT_WIDTH 2 //IN LOG(NUM_LINKS)
#define SERIALIZED_PACKET_WIDTH 16 //IN BITS
#define TAG_SIZE 11
#define PACKET_LNG 5
#define DEMUX_SELECT_WIDTH 3 //IN LOG(VAULTS_PER_QUADRANT)
#define ADDRESS_WIDTH 34 //IN BITS
#define CMD_WIDTH 7 //IN BITS
#define NUMBER_OF_CUBES 3
#define HEADER_WIDTH 64
#define TAIL_WIDTH 64
#define MAX_DATA_FLITS 16

#define RAS_LATENCY 8 //IN NS
#define CAS_LATENCY 2 //IN NS
#define FAW 4
#define CLOCK_RATE 1 //IN NS
#define VAULT_CLOCK_RATE 0.8 //IN NS
#define TRANS_CLOCK_RATE 0.03// IN NS




#define WR16  		"0001000"
#define WR32  		"0001001"
#define WR48  		"0001010"
#define WR64  		"0001011"
#define WR80  		"0001100"
#define WR96  		"0001101"
#define WR112 		"0001110"
#define WR128 		"0001111"
#define WR256 		"1001111"
#define MD_WR 		"0010000"
#define P_WR16  	"0011000"
#define P_WR32  	"0011001"
#define P_WR48  	"0011010"
#define P_WR64  	"0011011"
#define P_WR80  	"0011100"
#define P_WR96  	"0011101"
#define P_WR112 	"0011110"
#define P_WR128 	"0011111"
#define P_WR256 	"1011111"
#define RD16    	"0110000"
#define RD32    	"0110001"
#define RD48  		"0110010"
#define RD64  		"0110011"
#define RD80    	"0110100"
#define RD96    	"0110101"
#define RD112   	"0110110"
#define RD128   	"0110111"
#define RD256   	"1110111"
#define MD_RD   	"0101000"
#define ADD8		"0010010"
#define ADD16   	"0010011"
#define P_2ADD8 	"0100010"
#define P_ADD16	 	"0100011"
#define ADDS8R 		"1010010"
#define ADDS16R 	"1010011"
#define INC8 		"1010000"
#define P_INC8 		"1010100"
#define XOR16 		"1000000"
#define OR16 		"1000001"
#define NOR16 		"1000010"
#define AND16 		"1000011"
#define NAND16 		"1000100"
#define CASGT8 		"1100000"
#define CASGT16 	"1100010"
#define CASLT8 		"1100001"
#define CASLT16 	"1100011"
#define CASEQ8 		"1100100"
#define CASZERO16 	"1100101"
#define EQ8 		"1101001"
#define EQ16 		"1101000"
#define BWR 		"0010001"
#define P_BWR 		"0100001"
#define BWR8R 		"1010001"
#define SWAP16 		"1101010"

//PIM INSTRUCTIONS
#define PIM_INST	"0000001"

//PIM OPERATIONS SIZE
#define B16     "0000"
#define B32 	"0001"
#define B48 	"0010"
#define B64 	"0011"
#define B80 	"0100"
#define B96 	"0101"
#define B112 	"0110"
#define B128 	"0111"
#define B256 	"1000"

#endif
