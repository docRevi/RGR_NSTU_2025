#include "CodeWord_crypt.h"
#include <limits>
#include <iomanip>

vector<uint8_t> codeWordEncrypt(const vector<uint8_t> &bytes, const vector<uint8_t> &codeWord) {
    vector<uint8_t> encryptedBytes;
    encryptedBytes.reserve(bytes.size());

    const size_t codeWordSize = codeWord.size();

    for (size_t i = 0; i < bytes.size(); i++) {
        uint8_t encryptedByte = (bytes[i] + codeWord[i % codeWordSize]) % 256;
        encryptedBytes.emplace_back(encryptedByte);
    }

    return encryptedBytes;
}

vector<uint8_t> codeWordDecrypt(const vector<uint8_t> &bytes, const vector<uint8_t> &codeWord) {
    vector<uint8_t> decryptedBytes;
    decryptedBytes.reserve(bytes.size());

    const size_t codeWordSize = codeWord.size();

    for (size_t i = 0; i < bytes.size(); i++) {
        uint8_t decryptedByte = (bytes[i] - codeWord[i % codeWordSize]) % 256;
        decryptedBytes.emplace_back(decryptedByte);
    }

    return decryptedBytes;
}

void menu_CodeWordCrypt() {
    while (true) {
        clearScreen();
        cout << "\n=====Шифр с кодовым словом=====\n"
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
            cout << "Введите кодовое слово: ";
            string inputCodeWord;
            getline(cin, inputCodeWord);
            if (inputCodeWord.empty()) {
                throw runtime_error("Кодовое слово не может быть пустым");
            }

            vector<uint8_t> codeWord = stringToBytes(inputCodeWord);

            if (workMode == 1) {
                cout << "Введите текст в консоль: ";
                string consoleText;
                getline(cin, consoleText);
                vector<uint8_t> inputText = stringToBytes(consoleText);
                vector<uint8_t> result;
                if (do_encrypt) {
                    result = codeWordEncrypt(inputText, codeWord);
                }
                else {
                    result = codeWordDecrypt(inputText, codeWord);
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
                if (inputFile.empty()) {
                    throw runtime_error("Имя входного файла не может быть пустым");
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                string outputFile;
                cout << "Введите имя выходного файла: ";
                getline(cin, outputFile);
                if (outputFile.empty()) {
                    throw runtime_error("Имя выходного файла не может быть пустым");
                }
                vector<uint8_t> inputText = readFile(inputFile);
                if (do_encrypt) {
                    vector<uint8_t> result = codeWordEncrypt(inputText, codeWord);
                    writeFile(outputFile + "_enc", result);
                    cout << "Файл был успешно зашифрован и записан" << endl;
                    cout << "Для продолжения нажмите Enter" << endl;
                    cin.get();
                }
                else {
                    vector<uint8_t> result = codeWordDecrypt(inputText, codeWord);
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

