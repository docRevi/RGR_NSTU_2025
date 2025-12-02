#ifndef KEYWORD_CRYPT_H
#define KEYWORD_CRYPT_H

#include <iostream>
#include <cstdint>
#include <vector>
#include "../main/utils.h"

#ifdef __cplusplus
extern "C" {
#endif

vector<uint8_t> codeWordEncrypt(const vector<uint8_t> &bytes, const vector<uint8_t> &codeWord);
vector<uint8_t> codeWordDecrypt(const vector<uint8_t> &bytes, const vector<uint8_t> &codeWord);

void menu_CodeWordCrypt();

#ifdef __cplusplus
}
#endif

#endif 
