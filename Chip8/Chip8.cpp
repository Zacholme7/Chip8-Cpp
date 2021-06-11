#include "Chip8.h"

unsigned char fontSet[80] =
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


Chip8::Chip8()
{
	pc = 0x200;
	sp = 0;
	delayTimer = 0;
	soundTimer = 0;
	drawFlag = false;

	// clear the memory
	for (int i = 0; i < 4096; i++)
	{
		memory[i] = 0;
	}

	// clear the screen
	for (int i = 0; i < 2048; i++)
	{
		screen[i] = 0;
	}
	
	// load fontset into memory
	for (int i = 0; i < 80; i++)
	{
		memory[i] = fontSet[i];
	}

	// clear the stack and registers
	for (int i = 0; i < 16; i++)
	{
		registers[i] = 0;
		stack[i] = 0;
	}
}

void Chip8::loadRom(const std::string& fileName)
{
}

void Chip8::execute()
{
	/*
		fetch opcode from pc and pc+1
		increment pc by 2
	*/




	opcode = (memory[pc] << 8u) | (memory[pc + 1]);
	uint16_t nnn = opcode & 0x0FFF; 
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;
	uint8_t kk = opcode & 0x00FF;

	pc += 2;
	std::cout << "Curent Opcode: " << opcode << std::endl;

	switch (opcode & 0xF000)
	{
	case 0x0000:
		switch (opcode & 0x000F)
		{
		case 0x0000:
			// clear the display
			for (int i = 0; i < 2048; i++)
			{
				screen[i] = 0;
			}
			break;
		case 0x000E:
			// return from subroutine
			--sp;
			pc = stack[sp];
			break;
		default:
			std::cout << "Unknown opcode with leading 0" << std::endl;
			break;
		}
		break;
	case 0x1000:
		// Jump to location at nnn
		pc = nnn;
		break;

	case 0x2000:
		// Call subroutine at nnn
		stack[sp] = pc;
		++sp;
		pc = nnn;
		break;

	case 0x3000:
		// skip next instruction if Vx == kkk
		if (registers[vx] == kk)
		{
			pc += 2;
		}
		break;

	case 0x4000:
		// skip next instruction if Vx != kkk
		if (registers[vx] != kk)
		{
			pc += 2;
		}
		break;

	case 0x5000:
		// skip next instruction if Vx == Vy
		if (registers[vx] == registers[vy])
		{
			pc += 2;
		}
		break;

	case 0x6000:
		// set Vx == kk
		registers[vx] = kk;
		break;

	case 0x7000:
		// set Vx = Vx + kkk
		registers[vx] += kk;
		break;

	case 0x8000:
		// opcodes with leading 8 bit
		switch (opcode & 0x000F)
		{
		case 0x0000:
			// set Vx = Vy
			registers[vx] = registers[vy];
			break;

		case 0x0001:
			// set Vx = Vx OR Vy
			registers[vx] |= registers[vy];
			break;

		case 0x0002:
			// set Vx = Vx AND Vy
			registers[vx] &= registers[vy];
			break;

		case 0x0003:
			// set Vx = Vx XOR Vy
			registers[vx] ^= registers[vy];
			break;

		case 0x0004:
			// set VF == carry and Vx = Vx + Vy
			uint16_t result = registers[vx] + registers[vy];
			if (result > 255)
			{
				registers[0xF] = 1;
			}
			else
			{
				registers[0xF] = 0;
			}
			registers[vx] = result & 0xFF;
			break;

		case 0x0005:
			// set VF == (Vx > Vy) and Vx = Vx - Vy
			if (registers[vx] > registers[vy])
			{
				registers[0xF] = 1;
			}
			else
			{
				registers[0xF] = 0;
			}
			registers[vx] -= registers[vy];
			break;

		case 0x0006:
			// set Vf = LSB Vx, divide Vx by 2
			registers[0xF] = registers[vx] & 0x1;
			registers[vx] >>= 1;
			break;

		case 0x0007:
			// Vf = (Vy > Vx), Vx = Vy - Vx
			if (registers[vy] > registers[vx])
			{
				registers[0xF] = 1;
			}
			else
			{
				registers[0xF] = 0;
			}
			registers[vx] = registers[vy] - registers[vx];
			break;

		case 0x000E:
			// Vf = MSB Vx, multiply Vx by 2
			registers[0xF] = registers[vx] >> 7;
			registers[vx] <<= 1;
			break;

		default:
			std::cout << "Unknown opcode with leading 8 bit" << std::endl;
			break;
		}
		break;
		// end of opcodes with leading 8 bit

	case 0x9000:
		// skip next instruction if Vx != Vy
		if (registers[vx] != registers[vy])
		{
			pc += 2;
		}
		break;

	case 0xA000:
		// sets the indexReg == nnn
		indexReg = nnn;
		break;

	case 0xB000:
		// pc is set to nnn + v0
		pc = nnn + registers[0];
		break;

	case 0xC000:
		break;

	case 0xD000:
		break;

	case 0xE000:

		// opcodes with leading E bit
		switch(opcode & 0x000F)
		{
		case 0x000E:
			break;
		case 0x0001:
			break;
		default:
			std::cout << "Unknown opcode with leading E Bit" << std::endl;
			break;
		}
		break;

	case 0xF000:
		// opcodes with leading F bit
		switch (opcode & 0x00FF)
		{
		case 0x0007:
			// set Vx = delayTimer
			registers[vx] = delayTimer;
			break;
		case 0x000A:
			break;
		case 0x0015:
			// set delayTimer = Vx
			delayTimer = registers[vx];
			break;
		case 0x0018:
			// set soundTimer = Vx
			soundTimer = registers[vx];
			break;
		case 0x001E:
			// set indexReg to indexReg + Vx
			indexReg += registers[vx];
			break;
		case 0x0029:

			break;
		case 0x0033:
			break;
		case 0x0055:
			break;
		case 0x0065:
			break;
		}
		break;
	}




	
	if (delayTimer > 0)
	{
		--delayTimer;
	}

	if (soundTimer > 0)
	{
		--soundTimer;
	}
}
