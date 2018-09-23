#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "sm2.h"
#include "debug.h"

u8 _random[ECC_NUMWORD] = {
	0x6F, 0xF9, 0xB2, 0x1F, 0xAE, 0xBA, 0x0D, 0x26, 0x27, 0xB7, 0x72, 0xDD, 0x25, 0xD9, 0x76, 0xC1,
	0x3F, 0x66, 0x17, 0x48, 0x93, 0x4E, 0xF9, 0x94, 0x5C, 0x17, 0x5C, 0x38, 0x99, 0x8D, 0xB2, 0x6C};

ecc_point pub;
u8 pri[ECC_NUMWORD];

u8 hash[ECC_NUMWORD] = {
	0x95, 0x8E, 0x72, 0xE6, 0x3C, 0x1B, 0x65, 0xD3, 0x25, 0xAC, 0xF7, 0xF6, 0x50, 0xAF, 0xBA, 0x75,
	0x32, 0x5E, 0x22, 0x47, 0x58, 0xB0, 0x7C, 0x10, 0x66, 0xBB, 0xC1, 0x5A, 0xC5, 0x46, 0x89, 0xED};

u8 C[ECC_NUMWORD*5];
u8 M[ECC_NUMWORD*2] = {
	0x65, 0x6E, 0x63, 0x72,  0x79, 0x70, 0x74, 0x69,  0x6F, 0x6E, 0x20, 0x73,  0x74, 0x61, 0x6E, 0x64,
	0x61, 0x72, 0x64};

int main(int argc, char **argv)
{
	u8 r[ECC_NUMWORD];
	u8 s[ECC_NUMWORD];
	int ret = -1;
	int Clen;
	int Mlen;
	int loop = 1;

	if (argc > 1)
		loop = atoi(argv[1]);

	sm2_make_prikey(pri);
	sm2_make_pubkey(pri, &pub);

	sm2_sign(r, s, pri, hash);
	printHex("r", r, 32);
	printHex("s", s, 32);

	ret = sm2_verify(&pub, hash, r, s);
	if (ret)
		printf("verify err ret = %d\n", ret);
	else
		printf("verify ok\n");

	sm2_encrypt(&pub, M, 19, C, &Clen);
	printHex("C", C, Clen);
	memset(M, 0, 64);
	ret = sm2_decrypt(pri, C, Clen, M, &Mlen);
	if (ret)
		printf(" decrypt err \n");
	else
		printHex("M", M, Mlen);

	while (loop--) {
		speed_test("aa", 2);
		ret = sm2_verify(&pub, hash, r, s);
	}

	return 0;
}
