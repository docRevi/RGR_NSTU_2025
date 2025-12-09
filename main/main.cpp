#include <iostream>
#include <dlfcn.h>
#include "utils.h"
#include <limits>

typedef void (*MenuFunc)();

vector<void*> loadedHandles;

bool loadCipherLibrary(const string& libraryPath) {
    void* handle = dlopen(libraryPath.c_str(), RTLD_LAZY);
    if (!handle) {
        cerr << "Ошибка загрузки библиотеки " << libraryPath << ": " << dlerror() << endl;
        return false;
    }
    loadedHandles.push_back(handle);
    return true;
}

MenuFunc getMenuFunction(const string& functionName) {
    for (void* handle : loadedHandles) {
        MenuFunc func = (MenuFunc)dlsym(handle, functionName.c_str());
        if (func) return func;
    }
    return nullptr;
}

void unloadAllLibraries() {
    for (void* handle : loadedHandles) {
        if (handle) dlclose(handle);
    }
    loadedHandles.clear();
}

using namespace std;

int main() {
    setlocale(LC_ALL, "");
    clearScreen();

    // Загрузка библиотек
    string basePath = "./build/";
    if (!loadCipherLibrary(basePath + "libAtbash.so") ||
        !loadCipherLibrary(basePath + "libHill.so") ||
        !loadCipherLibrary(basePath + "libPolybius.so") ||
        !loadCipherLibrary(basePath + "libCodeWord.so")) {
        cerr << "Не удалось загрузить одну или несколько библиотек" << endl;
        return 1;
        }

    //Проверка выгрузки библиотек при выходе
    atexit(unloadAllLibraries);

    while (true) {
        try {
            cout << "Добро пожаловать в программу для работы с алгоритмами шифрования\n";
            cout << "Для работы потребуется ввести пароль и выбрать алгоритм для работы\n";
            manageWorkKey();
            clearScreen();
            cout << "Теперь выберите алгоритм для работы: \n";
            cout << "1 - Шифр Атбаш\n";
            cout << "2 - Шифр Хилла\n";
            cout << "3 - Шифр Полибия\n";
            cout << "4 - Шифр с кодовым словом\n";
            cout << "0 - Завершение работы\n";

            int input;
            cin >> input;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Ошибка ввода, введите число от 0 до 4");
            }

            char nextChar = cin.peek();
            if (nextChar != '\n' && nextChar != EOF){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Ошибка ввода, введите число от 0 до 4");
            }
            
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (input < 0 || input > 4) {
                throw invalid_argument("Ошибка выбора, введите число от 0 до 4");
            }
            if (input == 0) {
                break;
            }

            MenuFunc menuFunc = nullptr;
            string cipherName;

            switch (input) {
                case 1: {
                    menuFunc = getMenuFunction("menu_AtbashCrypt");
                    cipherName = "Атбаш";
                    break;
                }
                case 2: {
                    menuFunc = getMenuFunction("menu_HillCrypt");
                    cipherName = "Хилла";
                    break;
                }
                case 3: {
                    menuFunc = getMenuFunction("menu_PolybiusCrypt");
                    cipherName = "Полибия";
                    break;
                }
                case 4: {
                    menuFunc = getMenuFunction("menu_CodeWordCrypt");
                    cipherName = "c кодовым словом";
                    break;
                }
            }
            if (menuFunc) {
                cout << "\n=== Запуск шифра " << cipherName << " ===\n";
                menuFunc();
            } else {
                cerr << "Ошибка: функция меню не найдена для выбранного шифра" << endl;
            }

        }
        catch (exception& e) {
            cout << "Ошибка: " << e.what() << endl;
            cout << "Для продолжения нажмите Enter" << endl;
            cin.get();
        }
    }
    return 0;
}
