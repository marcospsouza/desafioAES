#include <gmp.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
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
	const char * const sX = "257794525404908372502154946820609836772";
	const char * const sY = "136894371430264143195294921195007788574";
	const char * const sMAX = "99999999999999999999999999999999999999999999999999";


	const char * const sprime1 = "5700734181645378434561188374130529072194886063077";
	const char * const sprime2 = "35894562752016259689151502540913447503526083242567";
	const char * const sprime3 = "5700734181645378434561188374130529072194886063076";
	const char * const sprime4 = "35894562752016259689151502540913447503526083242566";

	mpz_t n_primes;
	mpz_t on_primes;
	mpz_t prime1;
	mpz_t prime2;
	mpz_t prime3;
	mpz_t prime4;
	mpz_t public_key;
	mpz_t private_key;
	mpz_t mdc;
	mpz_t d;

	mpz_t n;
	mpz_t g;
	mpz_t X;
	mpz_t Y;
	mpz_t MAX;

	mpz_init(mdc);
	mpz_init(d);
	mpz_init(on_primes);
	mpz_init(n_primes);
	mpz_init(prime1);
	mpz_init(prime2);
	mpz_init(prime3);
	mpz_init(prime4);
	mpz_init(n);
	mpz_init(g);
	mpz_init(X);
	mpz_init(Y);
	mpz_init(MAX);

	if(mpz_set_str(prime3, sprime3, 10)) return -1;
	if(mpz_set_str(prime4, sprime4, 10)) return -1;
	if(mpz_set_str(prime1, sprime1, 10)) return -1;
	if(mpz_set_str(prime2, sprime2, 10)) return -1;
	if(mpz_set_str(n, sN, 10)) return -1;
	if(mpz_set_str(X, sX, 10)) return -1;
	if(mpz_set_str(Y, sY, 10)) return -1;
	if(mpz_set_str(MAX, sMAX, 10)) return -1;
	mpz_set_ui(g,2);


	mpz_mul(n_primes, prime1, prime2);
	mpz_mul(on_primes, prime3, prime4);

	int found_mdc = 1;
	int j = 2;
	mpz_t e;
	mpz_init(e);
	mpz_t um;
	mpz_init(um);
	mpz_set_ui(um, 1);
	while(found_mdc){
		mpz_set_ui(e, j);
		mpz_gcd(mdc, e, on_primes);
		gmp_printf("mdc %Zd\n", mdc);
		if(mpz_cmp(mdc, um))
			found_mdc = 0;
		j++;
	}
	mpz_invert(d, e, on_primes);

	gmp_printf("n: %Zd\n", n_primes);
	gmp_printf("O(n): %Zd\n", on_primes);
	gmp_printf("e: %Zd\n", e);
	gmp_printf("d: %Zd\n", d);

	mpz_t K;
	mpz_init(K);

	/*int flag = 1;
	for(int z = 1; flag && z < 58; z++){ // 7^z
		mpz_t a0;
		mpz_init(a0);
		mpz_ui_pow_ui(a0,7,z);
		for(int w = 1; flag && w < 70; w++){ // 5^w
			mpz_t a1;
			mpz_init(a1);
			mpz_ui_pow_ui(a1,5,w);
			mpz_mul(a1,a1,a0);

			if(mpz_cmp(MAX,a1) < 0)
				break;

			for(int y = 1; flag && y < 101; y++){ // 3^y
				mpz_t a2;
				mpz_init(a2);
				mpz_ui_pow_ui(a2,3,y);
				mpz_mul(a2,a2,a1);

				if(mpz_cmp(MAX,a2) < 0)
					break;

				for(int x = 1; flag && x < 160; x++){ // 2^x
					mpz_t a3;
					mpz_init(a3);
					mpz_ui_pow_ui(a3,2,x);
					mpz_mul(a3,a3,a2);

					if(mpz_cmp(MAX,a3) < 0)
						break;

					mpz_t a4;
					mpz_init(a4);
					mpz_powm(a4,g,a3,n);
					if(mpz_cmp(a4,X) == 0){
						gmp_printf("x: %Zd\n",a3);
						mpz_powm(K,Y,a3,n);
						flag = 0;
					}
				}
			}
		}
	}

	uint8_t key[16];
	mpz_export(key, NULL, -1, sizeof(uint8_t), 1, 0, K);

	printf("Texto decifrado com a chave '");
	for(int i = 0; i < 16; i++) printf("%c", key[i]);
	printf("':\n");

	uint8_t ciphertext[64][16];
	uint8_t plaintext[64][16];

	FILE *f = fopen("arquivo2.txt", "r+");
	for(int i = 0; i < 64; i++){
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

	for(int i = 0; i < 64; i++){
		AES_ECB_decrypt(ciphertext[i], key, plaintext[i], 16);
		for(int j = 0; j < 16; j++)
			printf("%c", plaintext[i][j]);
	}
	*/
}
