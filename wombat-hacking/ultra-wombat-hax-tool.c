#include <stdio.h>

// extremely crude program for reading out djmemc config registers
// I claim no copyright or license to this code

////////////////////////////////////////////////////////////////////////////////

#define PRINTF_BINARY_PATTERN_INT8 "%c%c%c%c%c%c%c%c"
#define PRINTF_BYTE_TO_BINARY_INT8(i)    \
    (((i) & 0x80ll) ? '1' : '0'), \
    (((i) & 0x40ll) ? '1' : '0'), \
    (((i) & 0x20ll) ? '1' : '0'), \
    (((i) & 0x10ll) ? '1' : '0'), \
    (((i) & 0x08ll) ? '1' : '0'), \
    (((i) & 0x04ll) ? '1' : '0'), \
    (((i) & 0x02ll) ? '1' : '0'), \
    (((i) & 0x01ll) ? '1' : '0')

#define PRINTF_BINARY_PATTERN_INT16 \
    PRINTF_BINARY_PATTERN_INT8              PRINTF_BINARY_PATTERN_INT8
#define PRINTF_BYTE_TO_BINARY_INT16(i) \
    PRINTF_BYTE_TO_BINARY_INT8((i) >> 8),   PRINTF_BYTE_TO_BINARY_INT8(i)
#define PRINTF_BINARY_PATTERN_INT32 \
    PRINTF_BINARY_PATTERN_INT16             PRINTF_BINARY_PATTERN_INT16
#define PRINTF_BYTE_TO_BINARY_INT32(i) \
    PRINTF_BYTE_TO_BINARY_INT16((i) >> 16), PRINTF_BYTE_TO_BINARY_INT16(i)

////////////////////////////////////////////////////////////////////////////////

#define MEMCAddr                 0x50F0E000               // same for both Wombat + WLCD memory maps
#define MEMCRegisterStart        0
#define MEMCIntleaveEnable       (MEMCRegisterStart)       // DRAM interleave enable reg.
#define dj_OneBufferedBusBit     5
//   djMEMC supports up to a maximum of 10 (TEN) DRAM banks
#define dj_MaxBanks              10                      //                                                       <H22>
#define MEMCDRAMCfg0             (MEMCIntleaveEnable+4)    // Bank 0 cfg reg
#define MEMCDRAMCfg1             (MEMCDRAMCfg0+4)          // Bank 1 cfg reg
#define MEMCDRAMCfg2             (MEMCDRAMCfg1+4)          // Bank 2 cfg reg
#define MEMCDRAMCfg3             (MEMCDRAMCfg2+4)          // Bank 3 cfg reg
#define MEMCDRAMCfg4             (MEMCDRAMCfg3+4)          // Bank 4 cfg reg
#define MEMCDRAMCfg5             (MEMCDRAMCfg4+4)          // Bank 5 cfg reg
#define MEMCDRAMCfg6             (MEMCDRAMCfg5+4)          // Bank 6 cfg reg
#define MEMCDRAMCfg7             (MEMCDRAMCfg6+4)          // Bank 7 cfg reg
#define MEMCDRAMCfg8             (MEMCDRAMCfg7+4)          // Bank 8 cfg reg
#define MEMCDRAMCfg9             (MEMCDRAMCfg8+4)          // Bank 9 cfg reg
#define MEMCmemTop               (MEMCDRAMCfg9+4)          // Top of stitched RAM
#define MEMCconfig               (MEMCmemTop+4)            // "Multiple bit DRAM/ROM configuration register"
#define MEMCRefresh              (MEMCconfig+4)            // DRAM refresh rate register

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	printf("ultra wombat hax tool\n");
	printf("by cy384\n");

	uint32_t reg;

	reg = *((uint32_t*)(MEMCAddr+MEMCIntleaveEnable)); printf("MEMCIntleaveEnable " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(reg));
	reg = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg0));       printf("MEMCDRAMCfg0       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(reg));
	reg = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg1));       printf("MEMCDRAMCfg1       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(reg));
	reg = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg2));       printf("MEMCDRAMCfg2       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(reg));
	reg = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg3));       printf("MEMCDRAMCfg3       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(reg));
	reg = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg4));       printf("MEMCDRAMCfg4       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(reg));
	reg = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg5));       printf("MEMCDRAMCfg5       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(reg));
	reg = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg6));       printf("MEMCDRAMCfg6       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(reg));
	reg = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg7));       printf("MEMCDRAMCfg7       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(reg));
	reg = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg8));       printf("MEMCDRAMCfg8       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(reg));
	reg = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg9));       printf("MEMCDRAMCfg9       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(reg));
	reg = *((uint32_t*)(MEMCAddr+MEMCmemTop));         printf("MEMCmemTop         " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(reg));
	reg = *((uint32_t*)(MEMCAddr+MEMCconfig));         printf("MEMCconfig         " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(reg));
	reg = *((uint32_t*)(MEMCAddr+MEMCRefresh));        printf("MEMCRefresh        " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(reg));

	printf("\ndone.\n(return to exit)\n");
	getchar();

	return 0;
}
