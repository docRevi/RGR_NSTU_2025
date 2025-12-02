#include "Atbash_crypt.h"
#include "../main/utils.h"
#include <limits>
#include <string>
#include <iomanip>

uint8_t atbashCryptByte(const uint8_t byte) {
    return ~byte;
    //return byte ^ 0xFF;
}

vector<uint8_t> atbashProcess(const vector<uint8_t>& bytes) {
    vector<uint8_t> outputBytes;
    outputBytes.reserve(bytes.size());

    for (auto byte: bytes) {
        outputBytes.push_back(atbashCryptByte(byte));
    }

    return outputBytes;
}

void menu_AtbashCrypt() {
    while (true) {
        setlocale(LC_ALL, "en_US.UTF-8");
        clearScreen();
        cout << "\n=====Шифр Атбаш=====\n"
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
                vector<uint8_t> result = atbashProcess(inputText);
                cout << (do_encrypt ? "Зашифрованный" : "Дешифрованный") << " текст (HEX): ";
                for (uint8_t byte : result) {
                    cout << hex << uppercase << setw(2) << setfill('0') << (int)byte << " ";
                }
                cout << dec << endl; // Сбросить формат обратно на десятичный
                cout << "Для продолжения нажмите Enter" << endl;
                cin.get();
            }
            else {
                cout << "Введите путь до входного файла: ";
                string inputFile;
                cin.clear();
                getline(cin, inputFile);
                
                string outputFile;
                cout << "Введите имя выходного файла: ";
                getline(cin, outputFile);
                vector<uint8_t> inputText = readFile(inputFile);
                if (do_encrypt) {
                    vector<uint8_t> result = atbashProcess(inputText);
                    writeFile(outputFile + "_enc", result);
                    cout << "Файл был успешно зашифрован и записан" << endl;
                    cout << "Для продолжения нажмите Enter" << endl;
                    cin.get();
                }
                else {
                    vector<uint8_t> result = atbashProcess(inputText);
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