#pragma once
#include "Operand.h"

ref class Processor;

enum class OperationType 
{ 
	NONE =				0,
	DATA_TRANSFER =		1, 
	ARITHMETIC =		2, 
	LOGICAL =			4,
	CONTROL_TRANSFER =	8,
	STACK_CONTROL =		16,
	MACHINE_CONTROL =	32
};

ref class Operation
{
public:

	OperationType type;
	delegate void operatingFunction(Operand^, Operand^, Processor^);
	operatingFunction^ fptr;

	Operation(void (*) (Operand^, Operand^, Processor^), OperationType);
	Operation(void (*) (Operand^, Operand^, Processor^), OperationType, OperandType);
	Operation(void (*) (Operand^, Operand^, Processor^), OperationType, OperandType, OperandType);
	Operation(const Operation% o);
	Operation();

	// Operand type information:
	OperandType expectedOperand1;
	OperandType expectedOperand2;
};