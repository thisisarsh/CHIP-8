/**
 * Class Members definition based on original CHIP-8 components
*/
#include<cstdint>
#include<fstream>

#define MSB_HI_NIBBLE(b) (((b) >> 12) & 0x0F)
#define MSB_LO_NIBBLE(b) (((b) >> 8) & 0x0F)
#define LSB_HI_NIBBLE(b) (((b) >> 4) & 0x0F)
#define LSB_LO_NIBBLE(b) ((b) & 0x0F)
#define LSB(b) ((b) & 0x00FF)
#define IM_MEMORY(b) ((b) & 0x0FFF)

class Chip8{
    public:
        uint8_t registers[16]{};
        uint8_t memory[4096]{};
        uint16_t index{};
        uint16_t pc{};
        uint16_t stack[16]{};
        uint8_t sp{};
        uint8_t delayTimer{};
        uint8_t soundTimer{};
        uint8_t keypad[16]{};
        uint32_t video[64*32]{};
        uint16_t opcode;

        Chip8();

        void LoadROM(char const*);

        void run();
};

const unsigned int START_ADDRESS = 0x200;

const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;

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

    for (unsigned int i = 0; i < FONTSET_SIZE; ++i){
        memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }

    pc = START_ADDRESS;
    index = 0;


}

void Chip8::run(){
    opcode = (memory[pc] << 8) | memory[pc + 1];
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
        /* code */
        break;
    case 0x3:
        /* code */
        break;
    case 0x4:
        /* code */
        break;
    case 0x5:
        /* code */
        break;

    case 0x6:
        registers[MSB_LO_NIBBLE(opcode)] = (LSB(opcode)); 
        break;

    case 0x7:
        registers[MSB_LO_NIBBLE(opcode)] += (LSB(opcode)); 
        break;
    case 0x8:
        /* code */
        break;
    case 0x9:
        /* code */
        break;
    case 0xA:
        index = IM_MEMORY(opcode);
        break;
    case 0xB:
        /* code */
        break;
    case 0xC:
        /* code */
        break;
    case 0xD:
        /* code */
        break;
    case 0xE:
        /* code */
        break;
    default:
        throw std::runtime_error("Unsupported opcode");
    }
}



void Chip8::LoadROM(char const* filename){
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if(file.is_open()){
        std::streampos size = file.tellg();

    }
}