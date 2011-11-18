#pragma once
#include "instruction.h"

#define REG_A 0
#define REG_B 1
#define REG_C 2
#define REG_D 3
#define REG_E 4
#define REG_H 5
#define REG_L 6

#define isRP(x) (((x)==REG_B)||((x)==REG_D)||((x)==REG_H))

ref class Executor
{
public:
	static String^ RegisterNames = "ABCDEHL";
	static int charToRegisterIndex(Char);
	
	void execute(Instruction^, Processor^);
};
