#pragma once
#include "Instruction.h"
#include "Executor.h"

ref class Parser
{
public:
	array<InstructionInfo^>^ operationTable;
	int operationsCount;
	static array<Char>^ separators = {' ', ','};

	Parser();
	void addOperation(String^, Operation%);
	Operand parseOperand(String^);
	Operation parseOperation(String^);
	Instruction^ parseLine(String^, int);
	static WORD parseNumber(String^);
};