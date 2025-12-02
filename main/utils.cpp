#include "utils.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>

void clearScreen() {
    system("clear");
}

vector<uint8_t> readFile(const string& filename) {
    ifstream file(filename, ios::binary | ios::ate);

    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл для чтения: " + filename);
    }

    std::streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    vector<uint8_t> buffer(size);

    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw runtime_error("Ошибка при чтении файла: " + filename);
    }

    return buffer;
}

void writeFile(const string& filename , const vector<uint8_t>& data) {
    ofstream file("files" + filename, ios::binary);

    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл для записи: " + filename);
    }

    // Записываем данные из вектора в файл
    if (!file.write(reinterpret_cast<const char*>(data.data()),  static_cast<streamsize>(data.size()))) {
        throw runtime_error("Ошибка при записи в файл: " + filename);
    }
}

vector<vector<int>> inputHillKey() {
    vector<vector<int>> key(2, vector<int>(2));

    cout << "Введите ключ-матрицу 2x2:" << endl;
    cout << "a b: "; cin >> key[0][0] >> key[0][1];
    cout << "c d: "; cin >> key[1][0] >> key[1][1];

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return key;
}

vector<uint8_t> stringToBytes(const string& text) {
    vector<uint8_t> bytes;
    bytes.reserve(text.size());
    for (const auto symbol: text) {
        bytes.push_back(static_cast<uint8_t>(symbol));
    }
    return bytes;
}

string bytesToString(const vector<uint8_t>& bytes) {
    string text;
    text.reserve(bytes.size());
    for (const auto byte: bytes) {
        text.push_back(static_cast<char>(byte));
    }
    return text;
}

string readFileToString(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return "";
    }

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

unsigned long hashKey(const string& Key) {
    unsigned long hash = 5381;

    for (char c : Key) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

void manageWorkKey() {
    while (true) {
        clearScreen();
        try {
            const string keyFilename("Password.txt");

            string storedHashStr = readFileToString(keyFilename);

            if (storedHashStr.empty()) {
                cout << "Это первый запуск работы с алгоритмами, давайте создадим пароль для будущей работы\n";
                cout << "Введите ваш пароль: ";
                string filePassword;
                getline(cin, filePassword);
                if (filePassword.empty()) {
                    throw invalid_argument("Пароль не может быть пустым");
                }

                unsigned long hashKeyValue = hashKey(filePassword);

                ofstream outFile(keyFilename);
                if (outFile.is_open()) {
                    outFile << hashKeyValue;
                    outFile.close();
                    cout << "Ключ захеширован и успешно сохранен в файл " << keyFilename << endl;
                } else {
                    cerr << "Ошибка: не удалось создать файл " << keyFilename << endl;
                }
            }
            else {
                cout << "Введите пароль для работы с алгоритмами: ";
                string inputKey;
                getline(cin, inputKey);
                
                unsigned long inputHash = hashKey(inputKey);
                
                unsigned long storedHash;
                try {
                    storedHash = stoul(storedHashStr);
                } catch (const exception& e) {
                    throw runtime_error("Ошибка чтения сохраненного пароля");
                }

                if (inputHash != storedHash) {
                    cout << "Неверный пароль, попробуйте снова" << endl;
                    cout << "Для продолжения нажмите Enter" << endl;
                    cin.get();
                    continue;
                }
            }
            break;
        }
        catch (exception& e) {
            cout << "Ошибка: " << e.what() << endl;
            cout << "Для продолжения нажмите Enter" << endl;
            cin.get();
        }
    }
}