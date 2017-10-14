#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <omp.h>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

using namespace std;

#define CBC 1
#define ECB 1

#include "aes.h"

int main(int argc, char* argv[]) {

#ifdef AES128
    printf("\nTesting AES128\n\n");
#elif defined(AES192)
    printf("\nTesting AES192\n\n");
#elif defined(AES256)
    printf("\nTesting AES256\n\n");
#else
    printf("You need to specify a symbol between AES128, AES192 or AES256. Exiting");
    return 0;
#endif

    uint8_t key[] = {'K', 'e', 'y', '2', 'G', 'r', 'o', 'u', 'p', '1', '4', 'X', 'X', 'X', 'X', 'X'};

    string sciphertext = "";
    uint8_t *ciphertext;
    uint8_t decryptedtext[16];

    char c;
    while(cin >> c && sciphertext.size() < 16){
        if(c >= '0' && c <= '9'){
            int d = (c - 48)*16;
            cin >> c;
            d += c - 48;
            sciphertext += d;
	   }   
    }    

    vector<uint8_t> myVector(sciphertext.begin(), sciphertext.end());
    ciphertext = &myVector[0];

    //while(ciphertext.size() % 16 != 0) ciphertext += '\0';

    cout << "COMEÇANDO" << endl;
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
                            char* filename = (char*) malloc(sizeof(char)*20);
                            sprintf(filename, "%d.%d.%d.%d.%d", key[11], key[12], key[13], key[14], key[15]);
                            ofstream f;
                            f.open(filename);
                            f << "Key: " << key << endl;
                            f << "Decrypted Text: " << endl;
                            f << decryptedtext;
                            f << endl << endl;
                            f.close();
                            free(filename);
			            }
			          
                    }
                }
            }
        }
    }

    return 0;
}
