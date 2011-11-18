#pragma once

/**
<summary>
	OperandType:
	MEMORY = M  (Register indirect addressing through H,L)
	REGISTER = [ABCDEHL]
	IMMEDIATE = 0x00 to 0xFF
	STACK_POINTER = SP
	PSW = PSW
	ADDRESS = 0x00 to 0xFFFF
</summary>
*/
[FlagsAttribute]
enum class OperandType:BYTE
{ 
	None =			0,
	Memory =		1,
	Register =		2,
	Immediate =		4,
	StackPointer =  8,
	PSW =			16,
	Address =		32
};

ref class Operand
{
public:
	static String^ validChars = "ABCDEFHLMPSW0123456789";

	OperandType type;
	WORD value;

	Operand(const Operand% o);
	Operand(OperandType, WORD);
};