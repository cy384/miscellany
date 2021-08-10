#include <stdio.h>
#include <Gestalt.h>

// crude program for reading out djmemc config registers
// I claim no copyright or license to this code

////////////////////////////////////////////////////////////////////////////////
// lifted from stack overflow

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
// values from "SuperMario" "HardwarePrivateEqu.r" file

#define MEMCAddr                 0x50F0E000
#define MEMCRegisterStart        0
#define MEMCIntleaveEnable       (MEMCRegisterStart)
#define MEMCDRAMCfg0             (MEMCIntleaveEnable+4)
#define MEMCDRAMCfg1             (MEMCDRAMCfg0+4)
#define MEMCDRAMCfg2             (MEMCDRAMCfg1+4)
#define MEMCDRAMCfg3             (MEMCDRAMCfg2+4)
#define MEMCDRAMCfg4             (MEMCDRAMCfg3+4)
#define MEMCDRAMCfg5             (MEMCDRAMCfg4+4)
#define MEMCDRAMCfg6             (MEMCDRAMCfg5+4)
#define MEMCDRAMCfg7             (MEMCDRAMCfg6+4)
#define MEMCDRAMCfg8             (MEMCDRAMCfg7+4)
#define MEMCDRAMCfg9             (MEMCDRAMCfg8+4)
#define MEMCmemTop               (MEMCDRAMCfg9+4)
#define MEMCconfig               (MEMCmemTop+4)
#define MEMCRefresh              (MEMCconfig+4)

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	printf("ultra wombat hax tool\n");
	printf("by cy384\n");

	uint32_t interleave = *((uint32_t*)(MEMCAddr+MEMCIntleaveEnable)); 
	uint32_t bank0 = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg0));
	uint32_t bank1 = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg1));
	uint32_t bank2 = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg2));
	uint32_t bank3 = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg3));
	uint32_t bank4 = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg4));
	uint32_t bank5 = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg5));
	uint32_t bank6 = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg6));
	uint32_t bank7 = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg7));
	uint32_t bank8 = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg8));
	uint32_t bank9 = *((uint32_t*)(MEMCAddr+MEMCDRAMCfg9));
	uint32_t top = *((uint32_t*)(MEMCAddr+MEMCmemTop));
	uint32_t config = *((uint32_t*)(MEMCAddr+MEMCconfig));
	uint32_t refresh = *((uint32_t*)(MEMCAddr+MEMCRefresh));

	long mach, physical_ram_size = 0;
	Gestalt(gestaltMachineType, &mach);
	Gestalt(gestaltPhysicalRAMSize, &physical_ram_size);

	printf("your machine reports being ");
	switch (mach)
	{
		case 30:
			printf("a Centris 650");
			break;
		case 35:
			printf("a Quadra 800");
			break;
		case 36:
			printf("a Quadra 650");
			break;
		case 51:
			printf("an unreleased \"speedbump\" wombat (Q650) (\"gestaltWombat40\")");
			break;
		case 59:
			printf("an unreleased \"speedbump\" wombat (Q800) (\"gestaltWombat40F\")");
			break;
		default:
			printf("???");
			break;
	}
	printf(" with %ld MB of RAM\n", (physical_ram_size / 1024) / 1024);

	printf("config set for: ");
	switch (config & 0b1111111111)
	{
		case 0b0100000001:
			printf("dj20Config or bump20Config");
			break;
		case 0b0000011010:
			printf("dj25Config or bump25Config");
			break;
		case 0b0010100011:
			printf("dj33Config");
			break;
		case 0b1011110100:
			printf("dj40Config");
			break;
		case 0b0011111011:
			printf("bump33Config");
			break;
		case 0b1011111100:
			printf("bump40Config");
			break;
		default:
			printf("???");
			break;
	}
	printf("\n");

	printf("config bits: ");
	if (config & 1<<3) printf("drpw "); 
	if (config & 1<<4) printf("drpchg ");
	if (config & 1<<5) printf("cyc2ta ");
	if (config & 1<<6) printf("drcpw ");
	if (config & 1<<7) printf("mhz33 ");
	if (config & 1<<8) printf("fastwr ");
	if (config & 1<<9) printf("dwcpw ");

	switch (config & 0b111)
	{
		case 0b001:
			printf("ROMspeed=1 ");
			break;
		case 0b010:
			printf("ROMspeed=2 ");
			break;
		case 0b011:
			printf("ROMspeed=3 ");
			break;
		case 0b100:
			printf("ROMspeed=4 ");
			break;
		default:
			printf("ROMspeed=??? ");
			break;
	}
	printf("\n");

	uint32_t refresh_cycles = (uint32_t)(0b1111111111 & refresh);
	int refresh_freq = (double)(refresh_cycles+27) / (double)15.6f;

	printf("refresh cycles: %lu (ideal for %d ish MHz)", refresh_cycles, refresh_freq);

	printf("\n(return to continue)\n");
	getchar();

	printf("RAM banks (small <= 32MB, large >= 64MB)\n");

	printf("interleaving on for banks: ");
	if (interleave & 1<<0) printf("0&1 ");
	if (interleave & 1<<1) printf("2&3 ");
	if (interleave & 1<<2) printf("4&5 ");
	if (interleave & 1<<3) printf("6&7 ");
	if (interleave & 1<<4) printf("8&9 ");
	printf("\n");

	printf("bank0 (%s): starting at %d MB\n", bank0 & 1 << 8 ? "small" : "large", (char)(bank0 & 0xff) * 4);
	printf("bank1 (%s): starting at %d MB\n", bank1 & 1 << 8 ? "small" : "large", (char)(bank1 & 0xff) * 4);
	printf("bank2 (%s): starting at %d MB\n", bank2 & 1 << 8 ? "small" : "large", (char)(bank2 & 0xff) * 4);
	printf("bank3 (%s): starting at %d MB\n", bank3 & 1 << 8 ? "small" : "large", (char)(bank3 & 0xff) * 4);
	printf("bank4 (%s): starting at %d MB\n", bank4 & 1 << 8 ? "small" : "large", (char)(bank4 & 0xff) * 4);
	printf("bank5 (%s): starting at %d MB\n", bank5 & 1 << 8 ? "small" : "large", (char)(bank5 & 0xff) * 4);
	printf("bank6 (%s): starting at %d MB\n", bank6 & 1 << 8 ? "small" : "large", (char)(bank6 & 0xff) * 4);
	printf("bank7 (%s): starting at %d MB\n", bank7 & 1 << 8 ? "small" : "large", (char)(bank7 & 0xff) * 4);
	printf("bank8 (%s): starting at %d MB\n", bank8 & 1 << 8 ? "small" : "large", (char)(bank8 & 0xff) * 4);
	printf("bank9 (%s): starting at %d MB\n", bank9 & 1 << 8 ? "small" : "large", (char)(bank9 & 0xff) * 4);

	printf("\n(return to continue)\n");
	getchar();

	printf("Raw values\n");
	printf("MEMCIntleaveEnable " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(interleave));
	printf("MEMCDRAMCfg0       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(bank0));
	printf("MEMCDRAMCfg1       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(bank1));
	printf("MEMCDRAMCfg2       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(bank2));
	printf("MEMCDRAMCfg3       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(bank3));
	printf("MEMCDRAMCfg4       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(bank4));
	printf("MEMCDRAMCfg5       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(bank5));
	printf("MEMCDRAMCfg6       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(bank6));
	printf("MEMCDRAMCfg7       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(bank7));
	printf("MEMCDRAMCfg8       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(bank8));
	printf("MEMCDRAMCfg9       " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(bank9));
	printf("MEMCmemTop         " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(top));
	printf("MEMCconfig         " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(config));
	printf("MEMCRefresh        " PRINTF_BINARY_PATTERN_INT32 "\n",PRINTF_BYTE_TO_BINARY_INT32(refresh));

	printf("\n");
	printf("gestaltPhysicalRAMSize %ld\n", physical_ram_size);
	printf("gestaltMachineType %ld\n", mach);

	printf("\n(return to exit)\n");
	getchar();

	return 0;
}
