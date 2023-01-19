#include <iostream>
#include "chip8.hpp"

int main(int argc, char* argv){
    if (argc != 4)
	{
		std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
		std::exit(EXIT_FAILURE);
	}

    Chip8 chip;
 
    /**
     * TO_DO:
     * T1. Take input as commands.
     * T2. Translate commands into opcode
     * T3. Run till exit
     * 
     *  \\*******************************\\
     *          CHIP-8 Executable
     * Set cycle speed (infinite cycle)
     * 
     *
     * 
    */
    
    return 0;
}