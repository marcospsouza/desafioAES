#include <gmp.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
// Enable both ECB and CBC mode. Note this can be done before including aes.h or at compile-time.
// E.g. with GCC by using the -D flag: gcc -c aes.c -DCBC=0 -DECB=1
#define CBC 1
#define ECB 1

#include "aes.h"

uint8_t ahex2bin (char MSB, char LSB){
	if (MSB > '9') MSB += 9;   // Convert MSB value to a contiguous range (0x30..0x?F)
	if (LSB > '9') LSB += 9;   // Convert LSB value to a contiguous range (0x30..0x?F)
	return (MSB << 4) | (LSB & 0x0F);   // Make a result byte using only low nibbles of MSB and LSB
}

int main(){
	const char * const sN = "340282366920938463463374607431768211297";
	const char * const sG = "339661812359158752487805590648382726653";
	const char * const sX = "214056462973616421506651988544363568686";
	const char * const sY = "187530560454113501587601065431781757109";

	mpz_t n;
	mpz_t g;
	mpz_t X;
	mpz_t Y;

	mpz_init(n);
	mpz_init(g);
	mpz_init(X);
	mpz_init(Y);

	if(mpz_set_str(n, sN, 10)) return -1;
	if(mpz_set_str(g, sG, 10)) return -1;
	if(mpz_set_str(X, sX, 10)) return -1;
	if(mpz_set_str(Y, sY, 10)) return -1;

	mpz_t _g;
	mpz_init(_g);
	mpz_invert(_g, g, n);

	mpz_t K;
	mpz_init(K);
	mpz_mul(K, X, Y);
	mpz_mul(K, K, _g);
	mpz_mod(K, K, n);

	uint8_t key[16];
	mpz_export(key, NULL, 1, sizeof(uint8_t), 1, 0, K);

	printf("Texto decifrado com a chave '");
	for(int i = 0; i < 16; i++) printf("%c", key[i]);
	printf("':\n");

	uint8_t ciphertext[92][16];
	uint8_t plaintext[92][16];

	FILE *f = fopen("arquivo1.txt", "r+");
	for(int i = 0; i < 92; i++){
		for(int j = 0; j < 16; j++){
			char a = fgetc(f);
			if(a == '\n')
				a = fgetc(f);
			char b = fgetc(f);
			if(b == '\n')
				b = fgetc(f);
			ciphertext[i][j] = ahex2bin(a,b);
		}
	}

	for(int i = 0; i < 92; i++){  
		AES_ECB_decrypt(ciphertext[i], key, plaintext[i], 16);
		for(int j = 0; j < 16; j++)
			printf("%c", plaintext[i][j]);
	}
}
