/**
 * Class Members definition based on original CHIP-8 components
*/
#include"chip8.hpp"
#include<cstdint>
#include<fstream>
#include<math.h>

#define MSB_HI_NIBBLE(b) (((b) >> 12) & 0x0F)
#define MSB_LO_NIBBLE(b) (((b) >> 8) & 0x0F)
#define LSB_HI_NIBBLE(b) (((b) >> 4) & 0x0F)
#define LSB_LO_NIBBLE(b) ((b) & 0x0F)
#define LSB(b) ((b) & 0x00FF)
#define IM_MEMORY(b) ((b) & 0x0FFF)

const uint32_t START_ADDRESS = 0x200;
const uint8_t FONTSET_SIZE = 80;
const uint32_t FONTSET_START_ADDRESS = 0x50;

Chip8::Chip8(){
    unsigned char fontset[FONTSET_SIZE] = 
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    for (uint8_t i = 0; i < FONTSET_SIZE; ++i){
        memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }

    pc = START_ADDRESS;
    index = 0;
    sp = 0;


}

void Chip8::run(){
    opcode = (memory[pc] << 8u) | memory[pc + 1];
    pc += 2;
    switch (MSB_HI_NIBBLE(opcode))
    {

    case 0x0:
        switch (LSB_HI_NIBBLE(opcode))
        {
        case 0xE0:
            memset(video, 0, sizeof(video));
            break;
        
        default:
            break;
        }
        break;

    case 0x1:
        pc = IM_MEMORY(opcode);
        break;

    case 0x2:
        stack[sp++] = pc -= 2;
        pc = IM_MEMORY(opcode);
        break;

    case 0x3:
        if(registers[MSB_LO_NIBBLE(opcode)] == LSB(opcode)){
            pc += 2;
        }
        break;

    case 0x4:
        if(registers[MSB_LO_NIBBLE(opcode)] != LSB(opcode)){
            pc += 2;
        }
        break;

    case 0x5:
        if(registers[MSB_LO_NIBBLE(opcode)] == registers[LSB_HI_NIBBLE(opcode)]){
            pc += 2;
        }
        break;

    case 0x6:
        registers[MSB_LO_NIBBLE(opcode)] = (LSB(opcode)); 
        break;

    case 0x7:
        registers[MSB_LO_NIBBLE(opcode)] += (LSB(opcode)); 
        break;
    case 0x8:
        switch (LSB_LO_NIBBLE(opcode))
        {
        case 0x0:
            registers[MSB_LO_NIBBLE(opcode)] = registers[LSB_HI_NIBBLE(opcode)];
            break;

        case 0x1:
            registers[MSB_LO_NIBBLE(opcode)] |= registers[LSB_HI_NIBBLE(opcode)];
            break;

        case 0x2:
            registers[MSB_LO_NIBBLE(opcode)] &= registers[LSB_HI_NIBBLE(opcode)];
            break;

        case 0x3:
            registers[MSB_LO_NIBBLE(opcode)] ^= registers[LSB_HI_NIBBLE(opcode)];
            break;

        case 0x4:
            registers[0x0F] = ((registers[MSB_LO_NIBBLE(opcode)] += registers[LSB_HI_NIBBLE(opcode)]) > 255) ? 1 : 0;
            break;

        case 0x5:
            registers[0x0F] = (registers[MSB_LO_NIBBLE(opcode)] > registers[LSB_HI_NIBBLE(opcode)]) ? 1 : 0;
            registers[MSB_LO_NIBBLE(opcode)] -= registers[LSB_HI_NIBBLE(opcode)];
            break;

        case 0x6:
            registers[0x0F] = (registers[LSB_HI_NIBBLE(opcode)] & 1);
            registers[MSB_LO_NIBBLE(opcode)] = (registers[LSB_HI_NIBBLE(opcode)] >> 1);
            break;

        case 0x7:
            registers[0x0F] = (registers[LSB_HI_NIBBLE(opcode)] > registers[MSB_LO_NIBBLE(opcode)]) ? 1 : 0;
            registers[LSB_HI_NIBBLE(opcode)] -= registers[MSB_LO_NIBBLE(opcode)];
            break;
        
        case 0xE:
            registers[0x0F] = (registers[LSB_HI_NIBBLE(opcode)] & 0x80);
            registers[MSB_LO_NIBBLE(opcode)] = (registers[LSB_HI_NIBBLE(opcode)] << 1);
            break;

        default:
            break;
        }
        break;

    case 0x9:
        if(registers[MSB_LO_NIBBLE(opcode)] != registers[LSB_HI_NIBBLE(opcode)]){
            pc += 2;
        }
        break;

    case 0xA:
        index = IM_MEMORY(opcode);
        break;

    case 0xB:
        /* BXNN */
        pc = registers[MSB_LO_NIBBLE(opcode)] + IM_MEMORY(opcode);
        break;

    case 0xC:
        registers[MSB_LO_NIBBLE(opcode)] = ((rand() % 0xFFFF) & LSB(opcode));
        break;

    case 0xD:
        int vx = registers[MSB_LO_NIBBLE(opcode)];
        int vy = registers[LSB_HI_NIBBLE(opcode)];
        int height = registers[LSB_LO_NIBBLE(opcode)];
        registers[0x0F] = 0; //Flag
        for(int i = 0 ; i < height ; i++){
            int pixel = memory[index + i];
            for (int j = 0; j < 8; j++){
                if((pixel & (0x80 >> j)) != 0){
                    if(video[(i + vy)*64 + (j + vx)] == 1){
                        registers[0x0F] = 1;
                    }
                    video[(i + vy)*64 + (j + vx)] ^= 1;
                }
            }
        }
        break;

    case 0xE:
        switch (LSB(opcode))
        {

        case 0x9E:
            if(keypad[registers[MSB_LO_NIBBLE(opcode)]] != 0){
                pc += 2;
            }
            break;
        
        case 0xA1:
            if(keypad[registers[MSB_LO_NIBBLE(opcode)]] == 0){
                pc += 2;
            }
            break;

        default:
            break;
        }
        break;

    case 0xF:
        switch (LSB(opcode))
        {
        case 0x07:
            registers[MSB_LO_NIBBLE(opcode)] = delayTimer;
            break;

        case 0x0A:
            pc -= 2u;
            for (uint8_t i = 0; i < 16; i++)
            {
                if (keypad[i] != 0)
                {
                    pc += 2u;
                    registers[MSB_LO_NIBBLE(opcode)] = i;
                }
            }
            break;
        
        case 0x15:
            delayTimer = registers[MSB_LO_NIBBLE(opcode)];
            break;

        case 0x18:
            soundTimer = registers[MSB_LO_NIBBLE(opcode)];
            break;

        case 0x1E:
            registers[0x0F] = (index += registers[MSB_LO_NIBBLE(opcode)]) > 0x0FFF ? 1 : 0;
            break;

        case 0x29:
            index = FONTSET_START_ADDRESS + ((registers[MSB_LO_NIBBLE(opcode)] & 0x0F) * 5);
            break;
        
        case 0x33:
            uint8_t num = registers[MSB_LO_NIBBLE(opcode)];
            for(uint8_t i = 0 ; num > 0 ; i++){
                memory[index + i] = (uint8_t)(num/pow(10,(3-i))) % 10;
            }
            break;

        case 0x55:
            for(uint8_t i = 0 ; i <= registers[MSB_LO_NIBBLE(opcode)] ; i++){
                memory[index + i] = registers[i];
            }
            break;

        case 0x65:
            for(uint8_t i = 0 ; i <= memory[index] ; i++){
                registers[i] = memory[index + i];
            }
            break;

        default:
            break;
        }
        break;

    default:
        throw std::runtime_error("Unsupported opcode");
    }
}



void Chip8::LoadROM(char const* filename){
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    std::ifstream obj();

    if(file.is_open()){
        std::streampos size = file.tellg();
		char* buffer = new char[size];

        for (long i = 0; i < size; ++i)
		{
			memory[START_ADDRESS + i] = buffer[i];
		}

        delete[] buffer;
    }
}