#include "flc.h"
#include "mxc_sys.h"
#include "cipher.h"
#include <stdio.h>
#include <string.h>

char computed_checksum[512];

void ChecksumVerify(unsigned char serialnumber[])
{
	char key[MXC_AES_KEY_128_LEN];
	char iv_dst[16];
	char msg[MXC_AES_DATA_LEN];

	memset(msg, 0, MXC_AES_DATA_LEN);
	memset(key, 0, MXC_AES_KEY_128_LEN);
	memset(iv_dst, 0, 16); 		

	memcpy(msg, serialnumber, 11);

	TPU_Cipher_Config(TPU_MODE_ECB, TPU_CIPHER_AES128);
	TPU_AES_Encrypt(msg, iv_dst, key, TPU_CIPHER_AES128, TPU_MODE_ECB, MXC_AES_DATA_LEN, computed_checksum);
}

int main(void) {
	unsigned int infoblock[6];
	unsigned char serialnumber[13];
	int batchinfo[5];
	int checksum[2];
	int dietyperev;
	int custominfo;
	int batchnum;
	int ylocation;
	int xlocation;
	int year;
	int fab;
	int wafer;

	printf("\n\n--------------------------------------\n");
	printf("Reading the Infoblock and Decoding USN\n");
	printf("Warning: This example only runs on TPU enabled parts\n\n");
	
	FLC_UnlockInfoBlock(0x10800000);
	// memcpy(*(uint32_t *)0x10800000, infoblock, 6);
	infoblock[0] = *(uint32_t *)0x10800000;
	infoblock[1] = *(uint32_t *)0x10800004;
	infoblock[2] = *(uint32_t *)0x10800008;
	infoblock[3] = *(uint32_t *)0x1080000C;
	infoblock[4] = *(uint32_t *)0x10800010;
	infoblock[5] = *(uint32_t *)0x10800014;
	FLC_LockInfoBlock(0x10800000);

	serialnumber[0] = (infoblock[0] & 0x007F8000) >> 15;
	serialnumber[1] = (infoblock[0] & 0x7F800000) >> 23;
	serialnumber[2] = (infoblock[1] & 0x0000007F) << 1;
	serialnumber[2] |= (infoblock[0] & 0x80000000) >> 31;
	serialnumber[3] = (infoblock[1] & 0x00007F80) >> 7;
	serialnumber[4] = (infoblock[1] & 0x007F8000) >> 15;
	serialnumber[5] = (infoblock[1] & 0x7F800000) >> 23;
	
	serialnumber[6] = (infoblock[2] & 0x007F8000) >> 15;
	serialnumber[7] = (infoblock[2] & 0x7F800000) >> 23;
	serialnumber[8] = (infoblock[3] & 0x0000007F) << 1;
	serialnumber[8] |= (infoblock[2] & 0x80000000) >> 31;
	serialnumber[9] = (infoblock[3] & 0x00007F80) >> 7;
	serialnumber[10] = (infoblock[3] & 0x007F8000) >> 15;
	serialnumber[11] = (infoblock[3] & 0x7F800000) >> 23;
	
	serialnumber[12] = (infoblock[4] & 0x007F8000) >> 15;

	dietyperev = serialnumber[0];
	custominfo = serialnumber[1];
	batchinfo[0] = serialnumber[2] & 0x3F;
	batchinfo[1] = (serialnumber[2] & 0xC0) >> 6;
	batchinfo[1] |= (serialnumber[3] & 0x0F) << 2;
	batchinfo[2] = (serialnumber[3] & 0xF0) >> 4;
	batchinfo[2] |= (serialnumber[4] & 0x03) << 4;
	batchinfo[3] = (serialnumber[4] & 0xFC) >> 2;
	batchinfo[4] = serialnumber[5]& 0x3F;
	
	batchnum = batchinfo[0] + batchinfo[1]*10 + batchinfo[2]*100;
	batchnum += batchinfo[3]*1000 + batchinfo[4]*10000;

	ylocation = serialnumber[6];
	ylocation |= (serialnumber[7] & 0x0F) << 8;
	xlocation = (serialnumber[7] & 0xF0) >> 4;	
	xlocation = serialnumber[8] << 4;
	year = serialnumber[9] & 0x1F;
	fab = (serialnumber[9] & 0x60) >> 5;
	wafer = (serialnumber[10] & 0x7E) >> 1;
	checksum[0] = serialnumber[11];
	checksum[1] = serialnumber[12];

	printf("Infoblock data\n");
	printf("0x%08x\t0x%08x\n", infoblock[0], infoblock[1]);
	printf("0x%08x\t0x%08x\n", infoblock[2], infoblock[3]);
	printf("0x%08x\t0x%08x\n", infoblock[4], infoblock[5]);
	printf("\n");

	printf("USN\n");
	printf("0x");
	for(int i = 0; i<13; i++) {
		printf("%02x", serialnumber[i]);
	}
	printf("\n\n");

	ChecksumVerify(serialnumber);
	if((computed_checksum[0] == checksum[1]) && (computed_checksum[1] == checksum[0])) {
		printf("Checksum Verified!\n");
	} else {
		printf("Invalid USN\n");
		while(1);
	}

	printf("Decoded USN\n");
	printf("Die Type Rev: %02x\n", dietyperev);
	printf("Custom Info : %02x\n", custominfo);
	printf("Foundry Char: %d\n", batchnum);
	printf("Wafer Number: %d\n", wafer);
	printf("X Location  : %d\n", xlocation);
	printf("Y Location  : %d\n", ylocation);
	printf("Year Built  : %d\n", year + 2010);
	printf("Fab ID Num  : %d\n", fab);
	printf("Checksum    : 0x%04x\n", checksum[0] | (checksum[1] << 8));

	while(1);
}