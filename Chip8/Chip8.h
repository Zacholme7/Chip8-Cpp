#ifndef chip8_h
#define chip8_h
#pragma once

#include <iostream>
#include <string>
#include <array>
class Chip8
{
public:
	Chip8();
	void loadRom(const std::string& fileName);
	void execute();
	bool getDrawflag() { return drawFlag; }
	void setKey(int i) { keypad[i] = 1; }
	void unsetKey(int i) { keypad[i] = 1; }
	bool drawFlag;
private:
	std::array<uint8_t, 4096> memory; // memory space
	std::array<uint8_t, 16> registers; // 16, 8bit registers
	std::array<uint16_t, 16> stack;
	std::array<uint16_t, 16> keypad;
	std::array<uint32_t, 2048> screen;
	uint8_t sp;				 // stack pointer
	uint16_t pc;			 // program counter starting at 0x200
	uint16_t indexReg;		 // index register
	uint8_t delayTimer;      // delay timer register
	uint8_t soundTimer;      // sound timer register
};



#endif
