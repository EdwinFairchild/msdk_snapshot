#include "flc.h"
#include "mxc_sys.h"
#include "cipher.h"
#include <stdio.h>
#include <string.h>

int ChecksumVerify(unsigned char serialnumber[])
{
	char result[512];
	char key[MXC_AES_KEY_128_LEN];
	char iv_dst[16];
	char msg[MXC_AES_DATA_LEN];

	memset(msg, 0, MXC_AES_DATA_LEN);
	memset(key, 0, MXC_AES_KEY_128_LEN);
	memset(iv_dst, 0, 16); 		

	memcpy(msg, serialnumber, 11);

	TPU_Cipher_Config(TPU_MODE_ECB, TPU_CIPHER_AES128);
	TPU_AES_Encrypt(msg, iv_dst, key, TPU_CIPHER_AES128, TPU_MODE_ECB, MXC_AES_DATA_LEN, result);

	if((result[0] == serialnumber[12]) && (result[1] == serialnumber[11])) {
		return E_NO_ERROR;
	} else {
		return E_INVALID;
	}
}

void GetUSN(unsigned char serialnumber[]) {
	unsigned int infoblock[6];
	
	FLC_UnlockInfoBlock(0x10800000);
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
}
int main(void) {

	unsigned char serialnumber[13];
	
	printf("\n\n--------------------------------------\n");
	printf("Reading the USN and verifying the checksum\n");
	printf("Warning: This example only runs on TPU enabled parts\n\n");
	
	GetUSN(serialnumber);

	printf("USN\n");
	printf("0x");
	for(int i = 0; i<13; i++) {
		printf("%02x", serialnumber[i]);
	}
	printf("\n");

	if(ChecksumVerify(serialnumber)) {
		printf("Invalid Checksum!\n");
	} else {
		printf("Checksum Valid!\n");
	}

	while(1);
}