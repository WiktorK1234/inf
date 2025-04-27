#ifndef UTILS_H
#define UTILS_H

#pragma once
#include <iostream>
#include <limits>

using namespace std;

inline void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

inline void czekajNaKlawisz() {
    cout << "\nNacisnij Enter, aby kontynuowac...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

#endif // UTILS_H