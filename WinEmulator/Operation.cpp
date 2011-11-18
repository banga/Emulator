#include "StdAfx.h"
#include "Operation.h"

Operation::Operation(void (*f)(Operand^, Operand^, Processor^), OperationType t)
{
	fptr = gcnew operatingFunction(f);
	type = t;
	expectedOperand1 = OperandType::None;
	expectedOperand2 = OperandType::None;
}
Operation::Operation(void (*f)(Operand^, Operand^, Processor^), OperationType t, OperandType op1)
{
	fptr = gcnew operatingFunction(f);
	type = t;
	expectedOperand1 = op1;
	expectedOperand2 = OperandType::None;
}
Operation::Operation(void (*f)(Operand^, Operand^, Processor^), OperationType t, OperandType op1, OperandType op2)
{
	fptr = gcnew operatingFunction(f);
	type = t;
	expectedOperand1 = op1;
	expectedOperand2 = op2;
}
Operation::Operation(const Operation% o)
{
	fptr = o.fptr;
	type = o.type;
	expectedOperand1 = o.expectedOperand1;
	expectedOperand2 = o.expectedOperand2;
}
Operation::Operation()
{
	type = OperationType::NONE;
}