#include <stdexcept>
#include <vector>
#include <cstdint>
#include <sstream>
#include <limits>
#include "Hill_crypt.h"

using namespace std;


string simpleHash(const string& input) {
    uint32_t hash = 0;

    for (char c : input) {
        hash = hash * 31 + static_cast<uint32_t>(c);
    }

    stringstream ss;
    ss << hex << setw(8) << setfill('0') << hash;

    string base = ss.str();
    string result;

    for (int i = 0; i < 8; i++) {
        uint32_t modified = hash * (i + 1) + i * 123;
        stringstream part;
        part << hex << setw(8) << setfill('0') << modified;
        result += part.str();
    }

    return result;
}

string calculateKeyHash(const vector<vector<int>>& key) {
    string keyData;
    for (const auto& row : key) {
        for (int val : row) {
            keyData += to_string(val);
        }
    }

    return simpleHash(keyData);
}

bool verifyKeyForFile(const string& filename,
                     const vector<vector<int>>& key) {

    ifstream file(filename, ios::binary);
    if (!file) return false;

    char storedHash[65];
    file.read(storedHash, 64);
    storedHash[64] = '\0';
    file.close();

    string calculatedHash = calculateKeyHash(key);
    return calculatedHash == string(storedHash);
}

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    throw runtime_error("Обратный элемент не существует");
}

vector<vector<int>> inverseMatrix(const vector<vector<int>>& matrix, int mod) {
    if (matrix.size() != 2 || matrix[0].size() != 2 || matrix[1].size() != 2) {
        throw invalid_argument("Матрица должна быть матрицей 2x2");
    }

    int a = matrix[0][0], b = matrix[0][1];
    int c = matrix[1][0], d = matrix[1][1];

    int det = (a * d - b * c) % mod;
    if (det < 0) det += mod;

    if (gcd(det, mod) != 1) {
        throw runtime_error("Определитель матрицы не взаимно прост с " + to_string(mod));
    }

    int detInv = modInverse(det, mod);

    vector<vector<int>> inv(2, vector<int>(2));
    inv[0][0] = (d * detInv) % mod;
    inv[0][1] = (-b * detInv) % mod;
    inv[1][0] = (-c * detInv) % mod;
    inv[1][1] = (a * detInv) % mod;
    
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            if (inv[i][j] < 0) {
                inv[i][j] += mod;
            }
        }
    }

    return inv;
}

vector<uint8_t> hillEncryptBytes(const vector<uint8_t>& plainBytes,
                                const vector<vector<int>>& key,
                                int mod = 256) {
    if (key.size() != 2 || key[0].size() != 2 || key[1].size() != 2) {
        throw invalid_argument("Ключ должен быть матрицей 2x2");
    }

    vector<uint8_t> cipherBytes;

    vector<uint8_t> paddedBytes = plainBytes;
    if (paddedBytes.size() % 2 != 0) {
        paddedBytes.push_back(0);
    }

    for (size_t i = 0; i < paddedBytes.size(); i += 2) {

        int p1 = paddedBytes[i] % mod;
        int p2 = paddedBytes[i + 1] % mod;

        int c1 = (key[0][0] * p1 + key[0][1] * p2) % mod;
        int c2 = (key[1][0] * p1 + key[1][1] * p2) % mod;

        if (c1 < 0) c1 += mod;
        if (c2 < 0) c2 += mod;

        cipherBytes.push_back(static_cast<uint8_t>(c1));
        cipherBytes.push_back(static_cast<uint8_t>(c2));
    }

    return cipherBytes;
}

vector<uint8_t> hillDecryptBytes(const vector<uint8_t>& cipherBytes, const vector<vector<int>>& key, int mod = 256) {
    if (key.size() != 2 || key[0].size() != 2 || key[1].size() != 2) {
        throw invalid_argument("Ключ должен быть матрицей 2x2");
    }

    if (cipherBytes.size() % 2 != 0) {
        throw invalid_argument("Длина шифротекста должна быть четной");
    }

    vector<vector<int>> invKey = inverseMatrix(key, mod);

    vector<uint8_t> plainBytes;

    for (size_t i = 0; i < cipherBytes.size(); i += 2) {
        int c1 = cipherBytes[i] % mod;
        int c2 = cipherBytes[i + 1] % mod;

        int p1 = (invKey[0][0] * c1 + invKey[0][1] * c2) % mod;
        int p2 = (invKey[1][0] * c1 + invKey[1][1] * c2) % mod;

        if (p1 < 0) p1 += mod;
        if (p2 < 0) p2 += mod;

        plainBytes.push_back(static_cast<uint8_t>(p1));
        plainBytes.push_back(static_cast<uint8_t>(p2));
    }

    while (!plainBytes.empty() && plainBytes.back() == 0) {
        plainBytes.pop_back();
    }

    return plainBytes;
}

void menu_HillCrypt() {
    while (true) {
        clearScreen();
        cout << "\n=====Шифр Хилла=====\n"
             << "1 - Шифрование\n"
             << "2 - Дешифрование\n"
             << "0 - Назад\n"
             << "Ваш выбор: " << endl;
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 0) break;
        if (choice < 1 or choice > 2) {
            cerr << "Некорректный выбор, введите число 0, 1 или 2" << endl;
            continue;
        }

        bool do_encrypt = (choice == 1);

        cout << "Выберите способ работы\n"
             << "1 - Ввод из терминала\n"
             << "2 - Ввод из файла\n"
             << "0 - Назад\n"
             << "Ваш выбор: " << endl;

        int workMode;
        cin >> workMode;

        if (workMode == 0) continue;
        if (workMode < 1 or workMode > 2) {
            cerr << "Некорректный выбор, введите число 0, 1 или 2" << endl;
            continue;
        }

        try {
            vector<vector<int>> key = inputHillKey();
            int det = key[0][0] * key[1][1] - key[0][1] * key[1][0];
            if (det % 2 == 0) {
                throw invalid_argument("Определитель ключа должен быть нечётным");
            }

            if (workMode == 1) {
                cout << "Введите текст в консоль: ";
                string consoleText;
                getline(cin, consoleText);

                vector<uint8_t> inputText = stringToBytes(consoleText);
                vector<uint8_t> result;

                if (do_encrypt) {
                    result = hillEncryptBytes(inputText,key);
                }
                else {
                    result = hillDecryptBytes(inputText,key);
                }
                cout << (do_encrypt ? "Зашифрованный" : "Дешифрованный") << " текст (HEX): ";
                for (uint8_t byte : result) {
                    cout << hex << uppercase << setw(2) << setfill('0') << (int)byte << " ";
                }
                cout << dec << endl;
                cout << "Для продолжения нажмите Enter" << endl;
                cin.get();
            }
            else {
                cout << "Введите путь до входного файла: ";
                string inputFile;
                getline(cin, inputFile);

                string outputFile;
                cout << "Введите имя выходного файла: ";
                getline(cin, outputFile);
                
                vector<uint8_t> inputText = readFile(inputFile);

                if (do_encrypt) {
                    vector<uint8_t> encryptedData = hillEncryptBytes(inputText, key);

                    ofstream file(outputFile + "_enc", ios::binary);
                    string keyHash = calculateKeyHash(key);
                    if (keyHash.size() != 64) {
                        throw runtime_error("Некорректный размер хеша");
                    }

                    file.write(keyHash.c_str(), 64);
                    file.write(reinterpret_cast<const char*>(encryptedData.data()),  static_cast<streamsize>(encryptedData.size()));
                    file.close();
                    cout << "Файл был успешно зашифрован и записан" << endl;
                    cout << "Для продолжения нажмите Enter" << endl;
                    cin.get();
                }
                else {
                    if (!verifyKeyForFile(inputFile, key)) {
                        throw invalid_argument("Хеши ключей не совпадают - неверный ключ!");
                    }

                    vector<uint8_t> fileContent = readFile(inputFile);

                    if (fileContent.size() <= 64) {
                        throw runtime_error("Файл слишком мал для дешифрования");
                    }
                    vector<uint8_t> encryptedData(fileContent.begin() + 64, fileContent.end());

                    vector<uint8_t> result = hillDecryptBytes(encryptedData, key);
                    writeFile(outputFile + "_dec", result);
                    cout << "Файл был успешно расшифрован и записан" << endl;
                    cout << "Для продолжения нажмите Enter" << endl;
                    cin.get();
                }
            }
        }
        catch (exception& e) {
            cerr << "Ошибка: " << e.what() << endl;
            cout << "Для продолжения нажмите Enter" << endl;
            cin.get();
        }
     }
}