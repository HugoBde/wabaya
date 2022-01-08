#ifndef MEMORY_H
#define MEMORY_H

#include <cstddef>
#include <string>

#include "utils.h"

static const int SYS_ROM_START              = 0x0000000;
static const int SYS_ROM_END                = 0x0003FFF;
static const int EWRAM_START                = 0x2000000;
static const int EWRAM_END                  = 0x203FFFF;
static const int IWRAM_START                = 0x3000000;
static const int IWRAM_END                  = 0x3007FFF;
static const int IO_RAM_START               = 0x4000000;
static const int IO_RAM_END                 = 0x40003FF;
static const int PAL_RAM_START              = 0x5000000;
static const int PAL_RAM_END                = 0x50003FF;
static const int VRAM_START                 = 0x6000000;
static const int VRAM_END                   = 0x6017FFF;
static const int OAM_START                  = 0x7000000;
static const int OAM_END                    = 0x70003FF;
static const int PAK_ROM_WAIT_STATE_0_START = 0x8000000;
static const int PAK_ROM_WAIT_STATE_0_END   = 0x9FFFFFF;
static const int PAK_ROM_WAIT_STATE_1_START = 0xA000000;
static const int PAK_ROM_WAIT_STATE_1_END   = 0xBFFFFFF;
static const int PAK_ROM_WAIT_STATE_2_START = 0xC000000;
static const int PAK_ROM_WAIT_STATE_2_END   = 0xDFFFFFF;
static const int CART_ROM_START             = 0xE000000;
static const int CART_ROM_END               = 0xE00FFFF;

class Memory {
    private:
    byte *sys_rom;
    byte *ewram;
    byte *iwram;
    byte *io_ram;
    byte *pal_ram;
    byte *vram;
    byte *oam;
    byte *pak_rom;
    byte *cart_rom;

    public:
    Memory();
    ~Memory();
    byte operator[](const size_t index);
    word get_word(const size_t index);
    halfword get_halfword(const size_t index);
    bool load_game(std::string filename);
    friend std::ostream &operator<<(std::ostream &os, const Memory &mem);
};

#endif