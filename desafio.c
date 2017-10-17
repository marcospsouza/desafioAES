#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>
#include <time.h>
// Enable both ECB and CBC mode. Note this can be done before including aes.h or at compile-time.
// E.g. with GCC by using the -D flag: gcc -c aes.c -DCBC=0 -DECB=1
#define CBC 1
#define ECB 1

#include "aes.h"

uint8_t ahex2bin (char MSB, char LSB)
{
    if (MSB > '9') MSB += 9;   // Convert MSB value to a contiguous range (0x30..0x?F)
    if (LSB > '9') LSB += 9;   // Convert LSB value to a contiguous range (0x30..0x?F)
    return (MSB << 4) | (LSB & 0x0F);   // Make a result byte using only low nibbles of MSB and LSB
}
int main(void)
{
    uint8_t ciphertext[16] = {0x84, 0xa8, 0x27, 0xb3, 0xe1, 0x8B, 0x85, 0xa9, 0x63, 0x5a, 0x93, 0x58, 0x2c, 0x96, 0xa7, 0x57};
    uint8_t plaintext[21][16]; //contando \n
    uint8_t plaintext_decrypted[21][16];
    //uint8_t key[] = {'e', 's', 's', 'a', 's', 'e', 'n', 'h', 'a', 'e', 'h', 'f', 'r', 'a', 'c', 'a'};
    uint8_t key[] = {'K', 'e', 'y', '2', 'G', 'r', 'o', 'u', 'p', '1', '4', 'X', 'X', 'X', 'X', 'X'};
    uint8_t full_key[] = {'K', 'e', 'y', '2', 'G', 'r', 'o', 'u', 'p', '1', '4', 'W', 'g', '1', 'p', '['};
    uint8_t decryptedtext[16];

    FILE *f = fopen("D5-GRUPO14.txt", "rb+");
    for(int i = 0; i < 21; i++){
        for(int j = 0; j < 16; j++){
            char a = fgetc(f);
            if(a == '\n')
                a = fgetc(f);
            char b = fgetc(f);
            if(b == '\n')
                b = fgetc(f);
            plaintext[i][j] = ahex2bin(a,b);
        }
    }

    printf("Texto decifrado com a chave 'Key2Group14WG1P[':\n");

    for(int i = 0; i < 21; i++){  
        AES_ECB_decrypt(plaintext[i], full_key, plaintext_decrypted[i], 16);
        for(int j = 0; j < 16; j++)
            printf("%c", plaintext_decrypted[i][j]);
    }

    printf("\n\nRodando algoritmo para descoberta da chave...\n");


    clock_t begin_time = clock();
    for(key[11] = 33; key[11] <= 126; key[11]++){
        for(key[12] = 33; key[12] <= 126; key[12]++){
            for(key[13] = 33; key[13] <= 126; key[13]++){
                for(key[14] = 33; key[14] <= 126; key[14]++){
                    #pragma omp parallel for
                    for(int k = 33; k <= 126; k++){
                        key[15] = k;
                        //
                        // Decrypt
                        //
                        AES_ECB_decrypt(ciphertext, key, decryptedtext, 16);
                
                        int alpha = 0;
                         for(int i = 0; i < 16; i++){
                            if( (decryptedtext[i] >= 65 && decryptedtext[i] <= 90) || (decryptedtext[i] >= 97 && decryptedtext[i] <= 122) 
                                || (decryptedtext[i] == 32)|| decryptedtext[i] == 'á' || decryptedtext[i] == 'à' || decryptedtext[i] == 'ã' 
                                || decryptedtext[i] == 'â' || decryptedtext[i] == 'é' || decryptedtext[i] == 'ê' || decryptedtext[i] == 'ú' 
                                || decryptedtext[i] == 'í' || decryptedtext[i] == 'ç' || decryptedtext[i] == 'Á' || decryptedtext[i] == 'À' 
                                || decryptedtext[i] == 'Ã' || decryptedtext[i] == 'Â' || decryptedtext[i] == 'É' || decryptedtext[i] == 'Ê' 
                                || decryptedtext[i] == 'Ú' || decryptedtext[i] == 'Í' || decryptedtext[i] == 'Ç' )
                                alpha++;
                        }
                        
                        if(alpha >= 14){
                            //
                            // Dump Decrypted Text
                            //
                            char name[30];
                            sprintf(name, "%d.%d.%d.%d.%d.txt", key[11], key[12], key[13], key[14], key[15]);
                           
                            FILE *log_file = fopen(name, "ab+");
                            
                            fprintf(log_file, "Time: %f\n", ((float) (clock() - begin_time))/CLOCKS_PER_SEC);
                            fprintf(log_file, "KEY: ");
                            for(int i = 0; i < 16; i++){
                                fprintf(log_file, "%c", key[i]);
                            }
                            fprintf(log_file, "\nDecrypted Text:\n");
                            for(int i = 0; i < 16; i++){
                                fprintf(log_file,"%c", decryptedtext[i]);
                            }
                            fclose(log_file);
                        }                    
                    }
                }
            }
        }
    }

    //test_decrypt_ecb();
    //test_encrypt_ecb();

    return 0;
}
