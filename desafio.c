#include <stdio.h>
#include <string.h>
#include <stdint.h>
// Enable both ECB and CBC mode. Note this can be done before including aes.h or at compile-time.
// E.g. with GCC by using the -D flag: gcc -c aes.c -DCBC=0 -DECB=1
#define CBC 1
#define ECB 1

#include "aes.h"

static void test_encrypt_ecb(void);
static void test_decrypt_ecb(void);


int main(void)
{
    uint8_t ciphertext[16];
    uint8_t key[] = {'K', 'e', 'y', '2', 'G', 'r', 'o', 'u', 'p', '1', '4', 'X', 'X', 'X', 'X', 'X'};
    uint8_t decryptedtext[16];


    FILE *f = fopen("D5-GRUPO14.txt", "r");
    int i;
    for(i = 0; i < 16; i++){
      ciphertext[i] = fgetc(f);
      printf("%c", ciphertext[i]);
    }
    printf("\nCOMEÇANDO\n");


    for(key[11] = 33; key[11] <= 126; key[11]++){
        for(key[12] = 33; key[12] <= 126; key[12]++){
            for(key[13] = 33; key[13] <= 126; key[13]++){
                for(key[14] = 33; key[14] <= 126; key[14]++){
                    for(int k = 33; k <= 126; k++){
                        key[15] = k;
                        //
                        // Decrypt
                        //
                        AES_ECB_decrypt(ciphertext, key, decryptedtext, 16);
                        /*CryptoPP::ECB_Mode< CryptoPP::AES >::Decryption ecbDecryption;
                        ecbDecryption.SetKey(key,sizeof(key));*/

                            
                        /*CryptoPP::StreamTransformationFilter stfDecryptor(ecbDecryption, new CryptoPP::StringSink( decryptedtext ) );
                      stfDecryptor.Put( reinterpret_cast<const unsigned char*>( ciphertext.c_str() ), ciphertext.size() );
                      stfDecryptor.MessageEnd();*/

                        int flag = 0;
                        for(int i = 0; i < 16; i++){
                            if(decryptedtext[i] < 0 || (decryptedtext[i] > 0 && decryptedtext[i] < 8) || (decryptedtext[i] > 13 && decryptedtext[i] < 32) || decryptedtext[i] > 126){
                                flag = 1; 
                                break;
                            }
                        }

                        if(flag==0){
                             //
                            // Dump Decrypted Text
                            //
                            char name[30];
                            sprintf(name, "%d.%d.%d.%d.%d.txt", key[11], key[12], key[13], key[14], key[15]);
                           
                            FILE *log_file = fopen(name, "ab+");

                            fprintf(log_file, "KEY: ");
                            for(int i = 0; i < 16; i++){
                                fprintf(log_file, "%c", key[i]);
                            }
                            fprintf(log_file, "\nDecrypted Text:\n");
                            for(int i = 0; i < 16; i++){
                                fprintf(log_file,"%c", decryptedtext[i]);
                            }
                            fclose(log_file);
                            /*f << "Key: " << key << endl;
                            f << "Decrypted Text: " << endl;
                            f << decryptedtext;
                            f << endl << endl;
                            f.close();
                            free(filename);*/
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

static void test_decrypt_ecb(void)
{
#ifdef AES128
    uint8_t key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    uint8_t in[] = {0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60, 0xa8, 0x9e, 0xca, 0xf3, 0x24, 0x66, 0xef, 0x97};
#elif defined(AES192)
  uint8_t key[] = { 0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5,
                    0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b};
  uint8_t in[] = { 0xbd, 0x33, 0x4f, 0x1d, 0x6e, 0x45, 0xf2, 0x5f, 0xf7, 0x12, 0xa2, 0x14, 0x57, 0x1f, 0xa5, 0xcc };
#elif defined(AES256)
    uint8_t key[] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                      0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };
    uint8_t in[] = { 0xf3, 0xee, 0xd1, 0xbd, 0xb5, 0xd2, 0xa0, 0x3c, 0x06, 0x4b, 0x5a, 0x7e, 0x3d, 0xb1, 0x81, 0xf8 };
#endif

  uint8_t out[]  = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};
  uint8_t buffer[16];

  AES_ECB_decrypt(in, key, buffer, 16);

  printf("ECB decrypt: ");

  if(0 == memcmp((char*) out, (char*) buffer, 16))
  {
    printf("SUCCESS!\n");
  }
  else
  {
    printf("FAILURE!\n");
  }
}



static void test_encrypt_ecb(void)
{
#ifdef AES128
    uint8_t key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
    uint8_t out[] = {0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60, 0xa8, 0x9e, 0xca, 0xf3, 0x24, 0x66, 0xef, 0x97};
#elif defined(AES192)
  uint8_t key[] = { 0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5,
                    0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b};
  uint8_t out[] = { 0xbd, 0x33, 0x4f, 0x1d, 0x6e, 0x45, 0xf2, 0x5f, 0xf7, 0x12, 0xa2, 0x14, 0x57, 0x1f, 0xa5, 0xcc };
#elif defined(AES256)
    uint8_t key[] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                      0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };
    uint8_t out[] = { 0xf3, 0xee, 0xd1, 0xbd, 0xb5, 0xd2, 0xa0, 0x3c, 0x06, 0x4b, 0x5a, 0x7e, 0x3d, 0xb1, 0x81, 0xf8 };
#endif

  uint8_t in[]  = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};
  uint8_t buffer[16];

  AES_ECB_encrypt(in, key, buffer, 16);

  printf("ECB encrypt: ");

  if(0 == memcmp((char*) out, (char*) buffer, 16))
  {
    printf("SUCCESS!\n");
  }
  else
  {
    printf("FAILURE!\n");
  }
}
