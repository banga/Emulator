#pragma once
#include "Parser.h"

ref class Processor
{
public:

	#define MAX_INSTRUCTIONS 100

	// [ A | B | C | D | E | H | L ]
	array<BYTE>^ REGISTERS;

	// Program Counter
	WORD PC;
	void jumpTo(WORD);

	// Base address of program
	WORD BASE_ADDRESS;

	// Set high when halted
	BYTE HALTED;

	//	Flags [ S | Z | - | AC | - | P | - | CY ]
	BYTE S, Z, AC, P, CY;

	void SET_CARRY(WORD);
	void SET_ZERO(WORD);
	void SET_SIGN(WORD);
	void SET_PARITY(WORD);
	void SET_AUX_CARRY(WORD, WORD);

	// M E M O R Y
	array<BYTE>^ memory;
	BYTE loadFromMemory();
	BYTE loadFromMemory(WORD);
	void storeToMemory(BYTE);
	void storeToMemory(WORD, BYTE);

	// S T A C K
	WORD SP;
	void push(WORD);
	WORD pop();

	//List of instructions to be executed
	InstructionList^ instructions;

	//Parser
	Parser parser;

	//Executor
	Executor executor;

	Processor();
	~Processor();

	// Reset the processor
	void RESET();
	
	// To be used in same order:
	void initializeOperations();

	void parse(array<String^>^);

	void executeOneStep();
	void execute();

	// External use:
	void displayRegisters(); // deprecated
	void syntaxError(char*);

	// Event handler for memory modifications
	delegate void MemoryAccessedAt(WORD);
	MemoryAccessedAt^ onMemoryReadAt;
	MemoryAccessedAt^ onMemoryWrittenAt;

	// To update form after each instruction
	delegate void InstructionExecuted(Instruction^);
	InstructionExecuted^ updateAfterStep;
};