#include "StdAfx.h"
#include "Instruction.h"

Instruction::Instruction(Operation op, int n)
{
	operation = gcnew Operation(op);
	lineNumber = n;
}

Instruction::Instruction(Operation op, Operand a, int n)
{
	operation = gcnew Operation(op);
	if(a.type != OperandType::None)
		op1 = gcnew Operand(a);
	lineNumber = n;
}

Instruction::Instruction(Operation op, Operand a, Operand b, int n)
{
	operation = gcnew Operation(op);
	if(a.type != OperandType::None)
		op1 = gcnew Operand(a);
	if(b.type != OperandType::None)
		op2 = gcnew Operand(b);
	lineNumber = n;
}

Byte Instruction::getSize()
{
	if(op1) {
		if(op1->type==OperandType::Immediate) {
			return 2;
		} else if(op1->type==OperandType::Address) {
			return 3;
		}		
		if(op2) {
			if(op2->type==OperandType::Immediate) {
				return 2;
			} else if(op2->type==OperandType::Address) {
				return 3;
			}
		}
	}

	return 1;
}

String^ Instruction::ToString()
{
	String^ s;

	s = "Line " + lineNumber + ": " + operation->fptr->Method->Name;

	if(op1) {
		s = s + " " + op1->type.ToString();

		if(op2) {
			s = s + ", " + op2->type.ToString();
		}
	}

	s = s + "(" + getSize() + " byte)";
	return s;
}
/* 
	InstructionList class 
*/
///<summary>
///<param name='size'>Size of the list</param>
///<param name='base_address'>Base address of the instructions</param>
///</summary>
InstructionList::InstructionList(Int32 size, WORD base_address) {
	instructions = gcnew array<Instruction^>(size);
	addresses = gcnew array<WORD>(size);

	addresses[0] = base_address;
	count = 0;
}

// Add(append) instruction to list of instructions to be executed
void InstructionList::addInstruction(Instruction^ i) {
	instructions[count] = i;

	if(count > 0)
		addresses[count] = addresses[count-1] + instructions[count-1]->getSize();

	count++;

	if(count > instructions->GetUpperBound(0)) {
		Array::Resize(instructions, count+16);
		Array::Resize(addresses, count+16);
	}
}

Instruction^ InstructionList::getInstructionAt(WORD address) {
	for(int i=0; i < count; i++) {
		if(addresses[i] == address) {
			return instructions[i];
		}
	}

	if(count == 0) {
		throw gcnew ArgumentException("There are no instructions in the instruction list.");
	} else if(instructions[count-1]->operation->type == OperationType::CONTROL_TRANSFER) {
		throw gcnew ArgumentException("Couldn't transfer control to " + Convert::ToString(address, 16)->PadLeft(4, '0') + ". No instruction was found there.");
	} else {
		throw gcnew ArgumentException("Instruction not found at address " + Convert::ToString(address, 16)->PadLeft(4, '0') + ". Use HLT at the end.");
	}
}
void InstructionList::clear() {
	for(int i=0; i < count; i++) {
		delete instructions[i];
	}
	count = 0;
}
InstructionList::~InstructionList() {
}