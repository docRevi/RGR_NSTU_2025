cmake_minimum_required(VERSION 3.30)
project(RGR)

set(CMAKE_CXX_STANDARD 20)

add_executable(RGR main/main.cpp
        Polybius/Polybius_crypt.cpp
        Polybius/Polybius_crypt.h
        Hill/Hill_crypt.cpp
        Hill/Hill_crypt.h
        Atbash/Atbash_crypt.cpp
        Atbash/Atbash_crypt.h
        CodeWord/CodeWord_crypt.cpp
        CodeWord/CodeWord_crypt.h
        main/utils.cpp
        main/utils.h
)
