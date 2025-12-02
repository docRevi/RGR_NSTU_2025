#ifndef HILL_CRYPT_H
#define HILL_CRYPT_H

#include <iostream>
#include "../main/utils.h"
#include <iomanip>
#include <fstream>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif 

vector<uint8_t> hillEncryptBytes(const vector<uint8_t>& plainBytes,
                                 const vector<vector<int>>& key,
                                 int mod);
vector<uint8_t> hillDecryptBytes(const vector<uint8_t>& cipherBytes,
                                 const vector<vector<int>>& key,
                                 int mod);
void menu_HillCrypt();

#ifdef __cplusplus
}
#endif

#endif
