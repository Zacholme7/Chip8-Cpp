#ifndef chip8_h
#define chip8_h
#pragma once

#include <iostream>
#include <string>
class Chip8
{
public:
	Chip8();
	void loadRom(const std::string& fileName);
	void execute();
	uint32_t screen[2048];
private:
	uint8_t memory[4096];    // 4Kb memory space
	uint8_t registers[16];   // 16, 8 bit registers
	uint8_t delayTimer;      // delay timer register
	uint8_t soundTimer;      // sound timer register
	uint8_t sp;				 // stack pointer
	uint8_t keypad[16];
	uint16_t stack[16];      // stack
	uint16_t pc;			 // program counter starting at 0x200
	uint16_t opcode;		 // the current opcode
	uint16_t indexReg;		 // index register
	bool drawFlag;
};



#endif
