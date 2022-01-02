#include <iostream>

#include "emulator.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: wabaya <rom filename>\n";
        std::cout << "Exiting\n";
        return 1;
    }
    Emulator emu = Emulator(argv[1]);
    emu.run();
    return 0;
}