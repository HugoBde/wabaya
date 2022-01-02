#include "emulator.h"

#include <fstream>
#include <iostream>

#include "utils.h"

Emulator::Emulator(std::string filename)
    : mem(), cpu(mem) {
    if (!mem.load_game(filename)) {
        log_error("Unable to load game");
    } else {
        log_success("Game successfully loaded");
    }
}

void Emulator::mem_dump() {
    std::ofstream dump;
    dump.open("dump_file", std::ios::out | std::ios::binary);
    if (dump.good()) {
        dump << mem;
    } else {
        log_warning("Unable to dump memory to dump_file");
    }
    dump.close();
}

void Emulator::run() {
    cpu.run();
}