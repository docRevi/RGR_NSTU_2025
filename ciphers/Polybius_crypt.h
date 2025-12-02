#ifndef POLYBIUS_CRYPT_H
#define POLYBIUS_CRYPT_H

#include <vector>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

    vector<uint8_t> polybius_encrypt(const vector<uint8_t>& text);
    vector<uint8_t> polybius_decrypt(const vector<uint8_t> &encryptedText);

void menu_PolybiusCrypt();

#ifdef __cplusplus
}
#endif

#endif
