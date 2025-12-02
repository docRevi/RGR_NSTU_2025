#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <cstdint>
#include <string>

using namespace std;

void clearScreen();

vector<vector<int>> inputHillKey();

vector<uint8_t> readFile(const string& filename);
void writeFile(const string& filename, const vector<uint8_t>& data);

vector<uint8_t> stringToBytes(const string& string);
string bytesToString(const vector<uint8_t>& bytes);

void manageWorkKey();


#endif //UTILS_H
