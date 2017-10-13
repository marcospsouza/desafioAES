#include <iostream>
#include <iomanip>
#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/filters.h>
#include <omp.h>
using namespace std;

int main(int argc, char* argv[]) {

    byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ] = {'K', 'e', 'y', '2', 'G', 'r', 'o', 'u', 'p', '1', '4', 'X', 'X', 'X', 'X', 'X'};

    string plaintext;
    string ciphertext = "";
    string decryptedtext;

    char c;
    while(cin >> c && ciphertext.size() < 16){
        if(c >= '0' && c <= '9'){
            int d = (c - 48)*16;
            cin >> c;
            d += c - 48;
            ciphertext += d;
	}
    }

    while(ciphertext.size() % 16 != 0) ciphertext += '\0';

    cout << ciphertext.size() << endl;

/*    for(key[11] = 33; key[11] <= 126; key[11]++){
        for(key[12] = 33; key[12] <= 126; key[12]++){
            for(key[13] = 33; key[13] <= 126; key[13]++){
                for(key[14] = 33; key[14] <= 126; key[14]++){
                    #pragma omp parallel for
                    for(key[15] = 33; key[15] <= 126; key[15]++){
*/
                        //
                        // Decrypt
                        //
                        CryptoPP::ECB_Mode< CryptoPP::AES >::Decryption ecbDecryption;
                        ecbDecryption.SetKey(key,sizeof(key));

			try{
                        CryptoPP::StreamTransformationFilter stfDecryptor(ecbDecryption, new CryptoPP::StringSink( decryptedtext ) );
                        stfDecryptor.Put( reinterpret_cast<const unsigned char*>( ciphertext.c_str() ), ciphertext.size() );
                        stfDecryptor.MessageEnd();
			} catch(CryptoPP::InvalidCiphertext e){
				cout << endl << e.GetWhat() << endl;
			}
    //
    // Dump Decrypted Text
    //
    cout << "Key: " << key << endl;
    cout << "Decrypted Text: " << endl;
    cout << decryptedtext;
    cout << endl << endl;
/*                    }
                }
            }
        }
    }
*/
    return 0;
}
