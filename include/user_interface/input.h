#pragma once

#include <iostream>

namespace input {

    inline void clear() {
        std::cin.ignore(69420, '\n');
    }

    inline std::string get() {
        std::string val{};
        std::getline(std::cin, val);
        return val;
    }

    template<typename T> inline T get(const bool loop = false) {
        bool fail{ false };
        do {
            fail = false;
            T val{};
            std::cin >> val;

            if (!std::cin) {
                std::cout << "Invalid input. Try again: ";
                std::cin.clear();
                clear();
                fail = true;
                continue;
            }
            else { clear(); return val; }
        } while (loop || fail);
        return T{}; //- ideally it should never reach this line
    }
}
