#pragma once

#include "operand.h"
#include "operation.h"

ref class Instruction
{
 public:

	Operand ^op1, ^op2;
	Operation ^operation;
	int lineNumber;

	Instruction(Operation, int);
	Instruction(Operation, Operand, int);
	Instruction(Operation, Operand, Operand, int);

	Byte getSize();
	virtual String^ ToString() override;
};

ref struct InstructionInfo
{
	String^ code;
	Operation^ operation;
};

ref class InstructionList
{
public:
	array<Instruction^>^ instructions;
	array<WORD>^ addresses;
	Int32 count;

	InstructionList(Int32, WORD);
	~InstructionList();

	void addInstruction(Instruction^);
	Instruction^ getInstructionAt(WORD);

	void clear();
};