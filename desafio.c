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


int main(void)
{
    uint8_t ciphertext[16] = {0x84, 0xa8, 0x27, 0xb3, 0xe1, 0x8B, 0x85, 0xa9, 0x63, 0x5a, 0x93, 0x58, 0x2c, 0x96, 0xa7, 0x57};
    //uint8_t key[] = {'e', 's', 's', 'a', 's', 'e', 'n', 'h', 'a', 'e', 'h', 'f', 'r', 'a', 'c', 'a'};
    uint8_t key[] = {'K', 'e', 'y', '2', 'G', 'r', 'o', 'u', 'p', '1', '4', 'X', 'X', 'X', 'X', 'X'};
    uint8_t decryptedtext[16];

       
    printf("\nCOMEÇANDO\n");


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
