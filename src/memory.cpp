#include "memory.h"

#include <fstream>
#include <iostream>
#include "utils.h"

Memory::Memory() {
    sys_rom  = new byte[0x4000];
    ewram    = new byte[0x40000];
    iwram    = new byte[0x8000];
    io_ram   = new byte[0x400];
    pal_ram  = new byte[0x400];
    vram     = new byte[0x18000];
    oam      = new byte[0x400];
    pak_rom  = new byte[0x2000000];
    cart_rom = new byte[0x10000];
    // load game data in pak rom, save data in cart_rom, zero out rest of memory except bios maybe
}

Memory::~Memory() {
    delete[] sys_rom;
    delete[] ewram;
    delete[] iwram;
    delete[] io_ram;
    delete[] pal_ram;
    delete[] vram;
    delete[] oam;
    delete[] pak_rom;
    delete[] cart_rom;
}

byte Memory::operator[](const size_t index) {
    if (SYS_ROM_START <= index && index <= SYS_ROM_END) {
        return sys_rom[index - SYS_ROM_START];
    } else if (EWRAM_START <= index && index <= EWRAM_END) {
        return ewram[index - EWRAM_START];
    } else if (IWRAM_START <= index && index <= IWRAM_END) {
        return iwram[index - IWRAM_START];
    } else if (IO_RAM_START <= index && index <= IO_RAM_END) {
        return io_ram[index - IO_RAM_START];
    } else if (PAL_RAM_START <= index && index <= PAL_RAM_END) {
        return pal_ram[index - PAL_RAM_START];
    } else if (VRAM_START <= index && index <= VRAM_END) {
        return vram[index - VRAM_START];
    } else if (OAM_START <= index && index <= OAM_END) {
        return oam[index - OAM_START];
    } else if (PAK_ROM_WAIT_STATE_0_START <= index && index <= PAK_ROM_WAIT_STATE_0_END) {
        return pak_rom[index - PAK_ROM_WAIT_STATE_0_START];
    } else if (PAK_ROM_WAIT_STATE_1_START <= index && index <= PAK_ROM_WAIT_STATE_1_END) {
        return pak_rom[index - PAK_ROM_WAIT_STATE_1_START];
    } else if (PAK_ROM_WAIT_STATE_2_START <= index && index <= PAK_ROM_WAIT_STATE_2_END) {
        return pak_rom[index - PAK_ROM_WAIT_STATE_2_START];
    } else if (CART_ROM_START <= index && index <= CART_ROM_END) {
        return cart_rom[index - CART_ROM_START];
    } else {
        log_error("Accessing invalid memory address");
        return 0;
    }
}

word Memory::get_word(const size_t index) {
    if (SYS_ROM_START <= index && index <= SYS_ROM_END) {
        return reinterpret_cast<word*>(sys_rom)[index - SYS_ROM_START];
    } else if (EWRAM_START <= index && index <= EWRAM_END) {
        return reinterpret_cast<word*>(ewram)[index - EWRAM_START];
    } else if (IWRAM_START <= index && index <= IWRAM_END) {
        return reinterpret_cast<word*>(iwram)[index - IWRAM_START];
    } else if (IO_RAM_START <= index && index <= IO_RAM_END) {
        return reinterpret_cast<word*>(io_ram)[index - IO_RAM_START];
    } else if (PAL_RAM_START <= index && index <= PAL_RAM_END) {
        return reinterpret_cast<word*>(pal_ram)[index - PAL_RAM_START];
    } else if (VRAM_START <= index && index <= VRAM_END) {
        return reinterpret_cast<word*>(vram)[index - VRAM_START];
    } else if (OAM_START <= index && index <= OAM_END) {
        return reinterpret_cast<word*>(oam)[index - OAM_START];
    } else if (PAK_ROM_WAIT_STATE_0_START <= index && index <= PAK_ROM_WAIT_STATE_0_END) {
        return reinterpret_cast<word*>(pak_rom)[index - PAK_ROM_WAIT_STATE_0_START];
    } else if (PAK_ROM_WAIT_STATE_1_START <= index && index <= PAK_ROM_WAIT_STATE_1_END) {
        return reinterpret_cast<word*>(pak_rom)[index - PAK_ROM_WAIT_STATE_1_START];
    } else if (PAK_ROM_WAIT_STATE_2_START <= index && index <= PAK_ROM_WAIT_STATE_2_END) {
        return reinterpret_cast<word*>(pak_rom)[index - PAK_ROM_WAIT_STATE_2_START];
    } else if (CART_ROM_START <= index && index <= CART_ROM_END) {
        return reinterpret_cast<word*>(cart_rom)[index - CART_ROM_START];
    } else {
        log_error("Accessing invalid memory address");
        return 0;
    }
}

halfword Memory::get_halfword(const size_t index) {
    if (SYS_ROM_START <= index && index <= SYS_ROM_END) {
        return reinterpret_cast<halfword*>(sys_rom)[index - SYS_ROM_START];
    } else if (EWRAM_START <= index && index <= EWRAM_END) {
        return reinterpret_cast<halfword*>(ewram)[index - EWRAM_START];
    } else if (IWRAM_START <= index && index <= IWRAM_END) {
        return reinterpret_cast<halfword*>(iwram)[index - IWRAM_START];
    } else if (IO_RAM_START <= index && index <= IO_RAM_END) {
        return reinterpret_cast<halfword*>(io_ram)[index - IO_RAM_START];
    } else if (PAL_RAM_START <= index && index <= PAL_RAM_END) {
        return reinterpret_cast<halfword*>(pal_ram)[index - PAL_RAM_START];
    } else if (VRAM_START <= index && index <= VRAM_END) {
        return reinterpret_cast<halfword*>(vram)[index - VRAM_START];
    } else if (OAM_START <= index && index <= OAM_END) {
        return reinterpret_cast<halfword*>(oam)[index - OAM_START];
    } else if (PAK_ROM_WAIT_STATE_0_START <= index && index <= PAK_ROM_WAIT_STATE_0_END) {
        return reinterpret_cast<halfword*>(pak_rom)[index - PAK_ROM_WAIT_STATE_0_START];
    } else if (PAK_ROM_WAIT_STATE_1_START <= index && index <= PAK_ROM_WAIT_STATE_1_END) {
        return reinterpret_cast<halfword*>(pak_rom)[index - PAK_ROM_WAIT_STATE_1_START];
    } else if (PAK_ROM_WAIT_STATE_2_START <= index && index <= PAK_ROM_WAIT_STATE_2_END) {
        return reinterpret_cast<halfword*>(pak_rom)[index - PAK_ROM_WAIT_STATE_2_START];
    } else if (CART_ROM_START <= index && index <= CART_ROM_END) {
        return reinterpret_cast<halfword*>(cart_rom)[index - CART_ROM_START];
    } else {
        log_error("Accessing invalid memory address");
        return 0;
    }
}

bool Memory::load_game(std::string filename) {
    std::ifstream file;  // we use ifstream (aka basic_ifstream<char>) instead of basic_ifstream<byte> (aka
                         // basic_ifstream<unsigned char>) because there is no trait implementation for unsigned char.
    file.open(filename, std::ios::in | std::ios::binary | std::ios::ate);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);
    if (file.good()) {
        file.read(reinterpret_cast<char*>(pak_rom), size);  // as a result, we reinterpret cast the unsigned char* to a char* to make things work
        file.close();
        return true;
    } else {
        file.close();
        return false;
    }
}

std::ostream& operator<<(std::ostream& os, const Memory& mem) {
    os.write(reinterpret_cast<char*>(mem.sys_rom), 0x4000);
    os.write(reinterpret_cast<char*>(mem.ewram), 0x40000);
    os.write(reinterpret_cast<char*>(mem.iwram), 0x8000);
    os.write(reinterpret_cast<char*>(mem.io_ram), 0x400);
    os.write(reinterpret_cast<char*>(mem.pal_ram), 0x400);
    os.write(reinterpret_cast<char*>(mem.vram), 0x18000);
    os.write(reinterpret_cast<char*>(mem.oam), 0x400);
    os.write(reinterpret_cast<char*>(mem.pak_rom), 0x2000000);
    os.write(reinterpret_cast<char*>(mem.cart_rom), 0x10000);
    return os;
}