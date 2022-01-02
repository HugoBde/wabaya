#ifndef EMULATOR_H
#define EMULATOR_H

#include <string>

#include "cpu.h"
#include "display.h"
#include "memory.h"
#include "soundsystem.h"

class Emulator {
    private:
    Memory mem;
    CPU cpu;

    public:
    Emulator(std::string filename);
    void mem_dump();
    void run();
};

#endif