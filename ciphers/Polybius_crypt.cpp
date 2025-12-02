#include <cstdint>
#include <vector>
#include <iostream>
#include <limits>
#include <iomanip>
#include "Polybius_crypt.h"
#include "../main/utils.h"
#include <stdexcept>

using namespace std;

vector<uint8_t> polybius_encrypt(const vector<uint8_t>& text) {
    vector<uint8_t> encryptedText;

    for (auto symbol: text){
        uint8_t row = symbol / 16;
        uint8_t col = symbol % 16;
        encryptedText.emplace_back(row);
        encryptedText.emplace_back(col);
    }

    return encryptedText;
}

vector<uint8_t> polybius_decrypt(const vector<uint8_t> &encryptedText) {
    vector<uint8_t> decryptedText;
    decryptedText.reserve(encryptedText.size() * 2);

    for (size_t i = 0; i < encryptedText.size(); i += 2) {
        const uint8_t row = encryptedText[i];
        const uint8_t col = encryptedText[i + 1];

        if (row > 15 or col > 15) {
            throw invalid_argument("Invalid Polybius coordinate");
        }

        uint8_t decryptSymbol = row * 16 + col;
        decryptedText.emplace_back(decryptSymbol);
    }

    return decryptedText;
}

void menu_PolybiusCrypt() {
    while (true) {
        clearScreen();
        cout << "\n=====Шифр Полибия=====\n"
             << "1 - Шифрование\n"
             << "2 - Дешифрование\n"
             << "0 - Назад\n"
             << "Ваш выбор: " << endl;
        int choice;
        cin >> choice;
        
        if (choice == 0) break;
        if (choice < 1 or choice > 2) {
            cerr << "Некорректный выбор, введите число 0, 1 или 2" << endl;
            continue;
        }

        bool do_encrypt = (choice == 1);

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Выберите способ работы\n"
             << "1 - Ввод из терминала\n"
             << "2 - Ввод из файла\n"
             << "0 - Назад\n"
             << "Ваш выбор: " << endl;

        int workMode;
        cin >> workMode;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (workMode == 0) continue;
        if (workMode < 1 or workMode > 2) {
            cerr << "Некорректный выбор, введите число 0, 1 или 2" << endl;
            continue;
        }

        try {
            if (workMode == 1) {
                cout << "Введите текст в консоль: ";
                string consoleText;
                getline(cin, consoleText);
                vector<uint8_t> inputText = stringToBytes(consoleText);
                vector<uint8_t> result;
                if (do_encrypt) {
                    result = polybius_encrypt(inputText);
                }
                else {
                    if (inputText.size() % 2 != 0) {
                        cout << "Ошибка: для дешифрования количество символов должно быть чётным." << endl;
                        cout << "Для продолжения нажмите Enter" << endl;
                        cin.get();
                        continue;
                    }
                    result = polybius_decrypt(inputText);
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
                    vector<uint8_t> result = polybius_encrypt(inputText);
                    writeFile(outputFile + "_enc", result);
                    cout << "Файл был успешно зашифрован и записан" << endl;
                    cout << "Для продолжения нажмите Enter" << endl;
                    cin.get();
                }
                else {
                    vector<uint8_t> result = polybius_decrypt(inputText);
                    writeFile(outputFile + "_dec", result);
                    cout << "Файл был успешно расшифрован и записан" << endl;
                    cout << "Для продолжения нажмите Enter" << endl;
                    cin.get();
                }
            }
        }
        catch (exception& e) {
            cout << "Ошибка: " << e.what() << endl;
            cout << "Для продолжения нажмите Enter" << endl;
            cin.get();
        }
     }
}