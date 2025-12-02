#ifndef ATBASH_CRYPT_H
#define ATBASH_CRYPT_H

#include <cstdint>
#include <vector>
#include <iostream>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

uint8_t atbashCryptByte(uint8_t byte);
vector<uint8_t> atbashProcess(const vector<uint8_t>& bytes);

void menu_AtbashCrypt();

#ifdef __cplusplus
}
#endif

#endif
