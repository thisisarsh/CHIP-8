#pragma once

#include <cstdint>

const uint16_t MEMORY_SIZE = 4096;
const uint16_t VIDEO_HEIGHT = 32;
const uint16_t VIDEO_WIDTH = 64;

class Chip8{
    private:
        uint8_t registers[16]{};
        uint8_t memory[MEMORY_SIZE]{};
        uint16_t index{};
        uint16_t pc{};
        uint16_t stack[16]{};
        uint8_t sp{};
        uint8_t delayTimer{};
        uint8_t soundTimer{};
        uint8_t keypad[16]{};
        uint32_t video[VIDEO_HEIGHT*VIDEO_WIDTH]{};
        uint16_t opcode{};

    public:

        Chip8();
        void LoadROM(char const*);
        void run();

};