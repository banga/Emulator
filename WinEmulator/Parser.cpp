#include "StdAfx.h"
#include "Parser.h"
#include "Executor.h"

#include "string.h"

// Static function:
WORD Parser::parseNumber(String^ op)
{
	int base = 10;

	if(op[op->Length-1] == 'H') {
		base = 16;
		op = op->Remove(op->Length-1);
	} else if(op[op->Length-1] == 'B') {
		base = 2;
		op = op->Remove(op->Length-1);
	}
	
	int value = 0;

	try {
		value = Convert::ToUInt16(op, base);
	} catch(ArgumentException^) {
		throw gcnew ArgumentException("Error parsing \'" + op + "\' - Only values from 0000h to FFFFh are allowed");
	} catch(FormatException^) {
		throw gcnew ArgumentException("Error parsing \'" + op + "\' - The operand contains invalid characters for base " + base + ". Use \'h\' for hex and \'b\' for binary numbers.");
	} catch(OverflowException^) {
		throw gcnew ArgumentException("Error parsing \'" + op + "\' - Only values from 0000h to FFFFh are allowed");
	}

	return value;
}

Parser::Parser()
{
	// Arbitrary large value used here
	operationTable = gcnew array<InstructionInfo^>(100);
	operationsCount = 0;
}
void Parser::addOperation(String^ s, Operation% o)
{
	operationTable[operationsCount] = gcnew InstructionInfo();
	operationTable[operationsCount]->code = gcnew String(s);
	operationTable[operationsCount++]->operation = gcnew Operation(o);
}
Operand Parser::parseOperand(String^ op)
{
	// Check for invalid characters
	for(int i=0; i < op->Length; i++) {
		if( Operand::validChars->IndexOf(op[i]) == -1 ) {
			throw gcnew ArgumentException("Invalid character '" + Convert::ToString(op[i]) + "' found while parsing operand.");
		}
	}

	if(op->Length == 1) {
		if(op[0] == 'M') {
			return Operand(OperandType::Memory, 0);
		}
		else {
			int index = Executor::charToRegisterIndex(op[0]);

			if(index != -1) {
				return Operand(OperandType::Register, index);
			}
		}
	} else if(op->CompareTo("SP")== 0) {
		return Operand(OperandType::StackPointer, 0);
	} else if(op->CompareTo("PSW")== 0) {
		return Operand(OperandType::PSW, 0);
	}

	// Throws exception
	WORD value = Parser::parseNumber(op);

	if((value >> 8) > 0)
		return Operand(OperandType::Address, value);
	
	return Operand(OperandType::Immediate, value);
}
Operation Parser::parseOperation(String^ code)
{
	// Parse operation:
	for(int k = 0; k < operationsCount; k++)
	{
		if(operationTable[k]->code == code)
			return *(operationTable[k]->operation);
	}

	throw gcnew ArgumentOutOfRangeException(code, "The given Opcode was not found in the operation table.");
}

Instruction^ Parser::parseLine(String^ line, int lineNumber)
{
	array<String^>^ temp = line->Split(Parser::separators);
	array<String^>^ split = gcnew array<String^>(4);

	int count = 0;
	for(int i = 0; i < temp->Length; i++) {
		if((temp[i]->Trim())->Length > 0) {
			split[count++] = temp[i];
			if(count > 3) {
				throw gcnew ArgumentException("The line has too many tokens.");
			}
		}
	}

	Operation operation = parseOperation(split[0]);
	String^ fname = operation.fptr->Method->Name;

	// Check for correct number of operands
	int expectedCount = 1;
	if(operation.expectedOperand1 != OperandType::None) expectedCount++;
	if(operation.expectedOperand2 != OperandType::None) expectedCount++;

	if(count != expectedCount) {
		throw gcnew ArgumentException(fname + " expects exactly " + (expectedCount-1).ToString() + " operands.");
	}

	if(count == 1) {
		return gcnew Instruction(operation, lineNumber);
	} else if(count == 2) {
		Operand operand = parseOperand(split[1]);

		// Check if operand should be an address
		if(operand.type == OperandType::Immediate && ((operation.expectedOperand1 & OperandType::Address) != OperandType::None)) {
			operand.type = OperandType::Address;
		}
		if((operand.type & operation.expectedOperand1) == OperandType::None) {
			throw gcnew ArgumentException(fname + " takes operand of type " + Convert::ToString(operation.expectedOperand1)->Replace(", ", " or "));
		}
		return gcnew Instruction(operation, operand, lineNumber);
	} else if(count == 3) {
		Operand op1 = parseOperand(split[1]);
		Operand op2 = parseOperand(split[2]);

		// Check if operand should be an address
		if(op1.type == OperandType::Immediate && ((operation.expectedOperand1 & OperandType::Address) != OperandType::None)) {
			op1.type = OperandType::Address;
		}
		if(op2.type == OperandType::Immediate && ((operation.expectedOperand2 & OperandType::Address) != OperandType::None)) {
			op2.type = OperandType::Address;
		}
		
		// Check if operand is an address but should be immediate data
		if( (op1.type == OperandType::Address && ((operation.expectedOperand1 & OperandType::Immediate) != OperandType::None))
 		 || (op2.type == OperandType::Address && ((operation.expectedOperand2 & OperandType::Immediate) != OperandType::None))) {
			throw gcnew ArgumentException(fname + " takes only byte-size data.");
		}

		if((op1.type & operation.expectedOperand1) == OperandType::None) {
			throw gcnew ArgumentException(fname + " takes first operand of type " + Convert::ToString(operation.expectedOperand1)->Replace(", ", " or "));
		}
		if((op2.type & operation.expectedOperand2) == OperandType::None) {
			throw gcnew ArgumentException(fname + " takes second operand of type " + Convert::ToString(operation.expectedOperand2)->Replace(", ", " or "));
		}

		return gcnew Instruction(operation, op1, op2, lineNumber);
	}

	throw gcnew ArgumentException("The line has too few tokens.");
}