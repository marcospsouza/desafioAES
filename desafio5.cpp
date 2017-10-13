#include <iostream>
#include <iomanip>
#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/filters.h>
using namespace std;

int main(int argc, char* argv[]) {

    byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE ];
    key[0] = 0x65; key[1] = 0x73; key[2] = 0x73; key[3] = 0x61; key[4] = 0x73; key[5] = 0x65; key[6] = 0x6e;
    key[7] = 0x68; key[8] = 0x61; key[9] = 0x65; key[10] = 0x68; key[11] = 0x66; key[12] = 0x72; key[13] = 0x61;
    key[14] = 0x63; key[15] = 0x61;
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );

    cout << endl << CryptoPP::AES::BLOCKSIZE << endl;

    //
    // String and Sink setup
    //
    string plaintext = "Texto para teste";
    string ciphertext;
    string decryptedtext;

    //
    // Dump Plain Text
    //
    cout << "Plain Text (" << plaintext.size() << " bytes)" << endl;
    cout << plaintext;
    cout << endl << endl;

    //
    // Create Cipher Text
    //
    //CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::ECB_Mode< CryptoPP::AES >::Encryption ecbEncryption;
    ecbEncryption.SetKey(key,sizeof(key));

    CryptoPP::StreamTransformationFilter stfEncryptor(ecbEncryption, new CryptoPP::StringSink( ciphertext ) );
    stfEncryptor.Put( reinterpret_cast<const unsigned char*>( plaintext.c_str() ), plaintext.length() + 1 );
    stfEncryptor.MessageEnd();

    //
    // Dump Cipher Text
    //
    cout << "Cipher Text (" << ciphertext.size() << " bytes)" << endl;

    for( int i = 0; i < ciphertext.size(); i++ ) {

        cout << "0x" << hex << (0xFF & static_cast<byte>(ciphertext[i])) << " ";
    }

    cout << endl << endl;

    //
    // Decrypt
    //
    CryptoPP::ECB_Mode< CryptoPP::AES >::Decryption ecbDecryption;
    ecbDecryption.SetKey(key,sizeof(key));

    CryptoPP::StreamTransformationFilter stfDecryptor(ecbDecryption, new CryptoPP::StringSink( decryptedtext ) );
    stfDecryptor.Put( reinterpret_cast<const unsigned char*>( ciphertext.c_str() ), ciphertext.size() );
    stfDecryptor.MessageEnd();

    //
    // Dump Decrypted Text
    //
    cout << "Decrypted Text: " << endl;
    cout << decryptedtext;
    cout << endl << endl;

    return 0;
}
