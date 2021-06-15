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


Chip8::Chip8():
	// value init arrays and init all other values
	memory(), 
	registers(),
	stack(), 
	keypad(), 
	screen(),
	sp(0),
	pc(0x200),
	delayTimer(0),
	soundTimer(0),
	drawFlag(false),
	indexReg(0)
{
	// load fontset into memory
	for (int i = 0; i < 80; i++)
	{
		memory[i] = fontSet[i];
	}
	srand(time(NULL));
}

void Chip8::loadRom(const std::string& fileName)
{
	std::ifstream romFile(fileName, std::ios::binary | std::ios::ate);
	std::ifstream::pos_type fileSize = romFile.tellg();
	char* fileBuffer = new char[fileSize];
	romFile.seekg(0, std::ios::beg);
	romFile.read(fileBuffer, fileSize);
	romFile.close();

	for (int i = 0; i < fileSize; i++)
	{
		memory[512 + i] = fileBuffer[i];
	}
	delete[] fileBuffer;
}

void Chip8::execute()
{
	uint16_t opcode = (memory[pc] << 8) | (memory[pc + 1]);
	uint16_t nnn = opcode & 0x0FFF; 
	uint8_t vx = (opcode & 0x0F00) >> 8;
	uint8_t vy = (opcode & 0x00F0) >> 4;
	uint8_t kk = opcode & 0x00FF;

	pc += 2;
	std::cout << "Curent Opcode: " << std::hex << opcode << std::endl;

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
		{
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
		}
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
			registers[0xF] = (registers[vx] & 0x80) >> 7;
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
		// register[vx] = random byte & kk 
		registers[vx] = (rand() % 255) & kk;
		break;

	case 0xD000:
	{
		uint8_t height = opcode & 0x000F; // the height of the sprite

		registers[0xF] = 0;
		for (int row = 0; row < height; row++)
		{
			uint8_t pxByte = memory[indexReg + row];

			for (int col = 0; col < 8; col++)
			{
				uint32_t &screenByte = screen[registers[vx] + col + ((registers[vy] + row) * 64)];
				if ((pxByte & (0x80 >> col)) != 0)
				{
					if (screenByte)
					{
						registers[0xF] = 1;
					}
					screenByte ^= 1;
				}
			}
		}
		drawFlag = true;
		break;
	}
	case 0xE000:

		// opcodes with leading E bit
		switch(opcode & 0x000F)
		{
		case 0x000E:
			// skip next instruction if val of vx == key pressed
			if (keypad[registers[vx]])
			{
				pc += 2;
			}
			break;
		case 0x0001:
			if (!keypad[registers[vx]])
			{
				pc += 2;
			}
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
		{
			bool waitingForKeyPress = true;
			for (int i = 0; i < 16; i++)
			{
				if (keypad[i] != 0)
				{
					registers[vx] = i;
					waitingForKeyPress = false;
					break;
				}
			}
			if (waitingForKeyPress)
			{
				pc -= 2;
			}
			break;
		}
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
			// set indexReg to location of sprite for digit VX
			indexReg = registers[vx] * 5;
			break;
		case 0x0033:
			// store hundreds, tens, ones place digets in i+2, i+1 and i
			memory[indexReg + 2] = registers[vx] % 10;
			memory[indexReg + 1] = (registers[vx] / 10) % 10;
			memory[indexReg] = registers[vx] % 100;
			break;
		case 0x0055:
			for (int i = 0; i <= vx; i++) 
			{
				memory[indexReg + i] = registers[i];
			}
			break;
		case 0x0065:
			for (int i = 0; i <= vx; i++)
			{
				registers[i] = memory[indexReg + i];
			}
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
