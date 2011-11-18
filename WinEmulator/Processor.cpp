#include "StdAfx.h"
#include "Processor.h"

Processor::Processor()
{
	REGISTERS = gcnew array<BYTE>(7);
	memory = gcnew array<BYTE>(65536);

	if(!REGISTERS || !memory) {
		throw gcnew SystemException("Cannot allocate memory on heap.");
	}

	RESET();
}

Processor::~Processor()
{
}

void Processor::RESET()
{
	BASE_ADDRESS = 0;
	HALTED = 0;
	PC = 0;
	SP = 0;
	S = Z = AC = P = CY = 0;

	for(int i=0; i < 7; i++)
		REGISTERS[i] = 0;
}

BYTE Processor::loadFromMemory() {
	return loadFromMemory( (REGISTERS[REG_H] << 8) + REGISTERS[REG_L]);
}
BYTE Processor::loadFromMemory(WORD address) {
	if(memory) {
		BYTE b = memory[address];

		if(onMemoryReadAt)
			onMemoryReadAt(address);
		return b;
	}
	return 0;
}
void Processor::storeToMemory(BYTE b) {
	storeToMemory((REGISTERS[REG_H]<<8) + REGISTERS[REG_L], b);
}
// Address low byte, Address hight byte, Immediate value
void Processor::storeToMemory(WORD address, BYTE b) {
	if(memory) {
		memory[address] = b;

		if(onMemoryWrittenAt)
			onMemoryWrittenAt(address);
	}
}
void Processor::push(WORD w) {
	if(instructions && SP > instructions->addresses[0] && SP <= instructions->addresses[instructions->count-1]+1) {
		syntaxError("Stack attempting to overwrite code. Please set SP before using push/pop.");
		return;
	}
	memory[--SP] = w >> 8;
	memory[--SP] = w & 0xFF;

	if(onMemoryWrittenAt) {
		onMemoryWrittenAt(SP+1);
		onMemoryWrittenAt(SP);
	}
}
WORD Processor::pop() {
	if(instructions && SP >= instructions->addresses[0]-1 && SP < instructions->addresses[instructions->count-1]) {
		syntaxError("Stack attempting to overwrite code. Please set SP before using push/pop.");
		return 0;
	}

	WORD x;
	x = memory[SP++];
	x += memory[SP++] << 8;

	if(onMemoryReadAt) {
		onMemoryReadAt(SP-2);
		onMemoryReadAt(SP-1);
	}

	return x;
}

// Parse given lines and append the parsed instructions to instruction list
void Processor::parse(array<String^>^ lines)
{
	if(!instructions) {
		throw gcnew ArgumentException("InstructionList* instructions list has not been initialized");
	}

	Instruction^ instruction;

	for(int i = 0; i < lines->Length; i++) {
		String^ line = lines[i]->Trim();

		if(line->Length==0) continue;

		//Throws exception:
		try{
			instruction = parser.parseLine(line->ToUpper(), i+1);
			instructions->addInstruction(instruction);
		} catch(ArgumentException^ e) {
			ArgumentException^ ae = gcnew ArgumentException(e->Message);
			ae->Data["LineNumber"] = i+1;
			throw ae;
		}
	}
}
/*
void Processor::parseFromFile(char* fname)
{
	FILE* fp;
	errno_t err = fopen_s(&fp, fname, "r");

	if(err != 0) {
		return;
	}

	char buffer[30];

	// Load base address of program:
	if(fgets(buffer, 30, fp)) {
		size_t len = strlen(buffer);
		if(buffer[len-1] == '\n')
			buffer[len-1] = 0;
		
		_strupr_s(buffer, 30);
		BASE_ADDRESS = Parser::parseNumber(buffer);

		if(BASE_ADDRESS < 0) {
			syntaxError("Theere was an error in parsing the base address\n");
			fclose(fp);
			return;
		}

		instructions = gcnew InstructionList(MAX_INSTRUCTIONS, BASE_ADDRESS);
	}
	else {
		syntaxError("Base address must be specified on first line of program\n");
		fclose(fp);
		return;
	}

	Instruction* instruction = 0;

	while(fgets(buffer, 30, fp)) {

		size_t len = strlen(buffer);
		if(len <= 1) continue;
		if(buffer[len-1] == '\n')
			buffer[len-1] = 0;

		//printf("Parsing '%s'...\n", buffer);
		_strupr_s(buffer, 30);
		instruction = parser.parseLine(buffer);

		if(instruction == 0) {
			printf("Parser returned null pointer at instructionCount = %d\n", instructions->count);
			break;
		}

		instructions.addInstruction(instruction);
	}
	
	fclose(fp);
}*/

// Execute single instruction
void Processor::executeOneStep()
{
	Instruction^ instruction;
	
	try {
		instruction = instructions->getInstructionAt(PC);
	} catch(ArgumentException^ e) {
		if(instructions->count)
			e->Data["LineNumber"] = instructions->instructions[instructions->count-1]->lineNumber;
		throw e;
	}


	try {
		executor.execute(instruction, this);
	} catch(ArgumentException^ e) {		
		e->Data["LineNumber"] = instruction->lineNumber;
		throw e;
	}

	if(instruction->operation->type != OperationType::CONTROL_TRANSFER && !HALTED) {
		PC += instruction->getSize();
	}

	//Used for updating forms
	if(updateAfterStep)
		updateAfterStep(instruction);
}

// Execute the program from base address till it halts
void Processor::execute()
{	
	while(!HALTED)
		executeOneStep();
}

void Processor::displayRegisters()
{
	/*	
	printf(" A | B | C | D | E | H | L |  PC  |  SP  | S Z AC P CY |\n");

	for(int i = 0; i < 7; i++)
		printf(" %.2X ", REGISTERS[i]);

	printf(" %.4X  ", PC);
	printf(" %.4X  ", SP);
	printf(" %d %d  %d %d  %d", S, Z, AC, P, CY);

	printf("\n");
	*/
	throw gcnew System::Exception("displayRegisters is set for deletion.");
}

/*
       	 D A T A  T R A N S F E R  G R O U P
(MOV, MVI, LDA, STA, LHLD, SHLD, XCHG, LXI, LDAX, STAX)
*/
void MOV(Operand^ o1, Operand^ o2, Processor^ p)
{
	if((o1->type == OperandType::Memory && o2->type != OperandType::Register)) {
		p->syntaxError("When MOVing to memory, second operand can only be register");
		return;
	} else if((o2->type == OperandType::Memory && o1->type != OperandType::Register)) {
		p->syntaxError("When MOVing from memory, first operand can only be register");
		return;
	}

	/*
		Get value of RHS:
	*/
	BYTE rvalue = 0;

	if(o2->type == OperandType::Register) {
		rvalue = p->REGISTERS[o2->value];
	}
	else if(o2->type == OperandType::Memory) {
		rvalue = p->loadFromMemory();
	}

	/*
		Assign rvalue to operand1 
	*/
	if(o1->type == OperandType::Register) {
		p->REGISTERS[o1->value] = rvalue;
	}
	else if(o1->type == OperandType::Memory) {
		p->storeToMemory(rvalue);
	}
}

void MVI(Operand^ o1, Operand^ o2, Processor^ p)
{
	if(o1->type == OperandType::Register)
		p->REGISTERS[o1->value] = (BYTE)o2->value;
	else
		p->storeToMemory((BYTE)o2->value);
}

void LDA(Operand^ o1, Operand^ o2, Processor^ p)
{
	p->REGISTERS[REG_A] = p->loadFromMemory(o1->value);
}

void STA(Operand^ o1, Operand^ o2, Processor^ p)
{
	p->storeToMemory(o1->value, p->REGISTERS[REG_A]);
}

void LHLD(Operand^ o1, Operand^ o2, Processor^ p)
{
	p->REGISTERS[REG_L] = p->loadFromMemory(o1->value);
	p->REGISTERS[REG_H] = p->loadFromMemory(o1->value+1);
}

void SHLD(Operand^ o1, Operand^ o2, Processor^ p)
{
	p->storeToMemory(o1->value, p->REGISTERS[REG_L]);
	p->storeToMemory(o1->value+1, p->REGISTERS[REG_H]);
}

void XCHG(Operand^ o1, Operand^ o2, Processor^ p)
{
	BYTE t = p->REGISTERS[REG_H];
	p->REGISTERS[REG_H] = p->REGISTERS[REG_D];
	p->REGISTERS[REG_D] = t;

	t = p->REGISTERS[REG_L];
	p->REGISTERS[REG_L] = p->REGISTERS[REG_E];
	p->REGISTERS[REG_E] = t;
}

void LXI(Operand^ o1, Operand^ o2, Processor^ p)
{
	if(o1->type == OperandType::Register) {
		if(!isRP(o1->value)) {
			p->syntaxError("First operand to LXI must be a register pair [B/D/H]");
			return;
		}

		p->REGISTERS[o1->value] = o2->value >> 8;
		p->REGISTERS[o1->value+1] = o2->value & 0x00FF;
	} else {
		p->SP = o2->value;
	}
}

void LDAX(Operand^ o1, Operand^ o2, Processor^ p)
{
	if(o1->value != REG_B && o1->value != REG_D) {
		p->syntaxError("First operand to LDAX must be a register pair [B/D]");
		return;
	}

	p->REGISTERS[REG_A] = p->loadFromMemory( (p->REGISTERS[o1->value] << 8) + p->REGISTERS[o1->value+1]);
}

void STAX(Operand^ o1, Operand^ o2, Processor^ p)
{
	if(o1->value != REG_B && o1->value != REG_D) {
		p->syntaxError("First operand to STAX must be a register pair [B/D]");
		return;
	}

	p->storeToMemory( (p->REGISTERS[o1->value] << 8) + p->REGISTERS[o1->value+1], p->REGISTERS[REG_A]);
}
/*
	          E N D  O F
	D A T A  T R A N S F E R  G R O U P
*/

/*
					A R I T H M E T I C  G R O U P
(ADD, ADC, ADI, ACI, SUB, SBB, SUI, SBI, INR, INX, DCR, DCX, DAD, DAA)
*/
void ADD(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD result, x = p->REGISTERS[REG_A], y;

	if(o1->type == OperandType::Register)
		y = p->REGISTERS[o1->value];
	else
		y = p->loadFromMemory();

	result = x + y;

	// Flags:
	p->SET_CARRY(result);
	p->SET_PARITY(result);
	p->SET_AUX_CARRY(x,y);
	p->SET_ZERO(result);
	p->SET_SIGN(result);

	p->REGISTERS[REG_A] = (BYTE)result;
}

void ADC(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD result, x = p->REGISTERS[REG_A], y;

	if(o1->type == OperandType::Register)
		y = p->REGISTERS[o1->value];
	else
		y = p->loadFromMemory();

	result = x + y + p->CY;

	// Flags:
	p->SET_CARRY(result);
	p->SET_PARITY(result);
	p->SET_AUX_CARRY(x,y);
	p->SET_ZERO(result);
	p->SET_SIGN(result);

	p->REGISTERS[REG_A] = (BYTE)result;
}

void ADI(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD result, x = p->REGISTERS[REG_A], y = o1->value;
	result = x + y;

	// Flags:
	p->SET_CARRY(result);
	p->SET_PARITY(result);
	p->SET_AUX_CARRY(x,y);
	p->SET_ZERO(result);
	p->SET_SIGN(result);

	p->REGISTERS[REG_A] = (BYTE)result;
}

void ACI(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD result, x = p->REGISTERS[REG_A], y = o1->value;
	result = x + y + p->CY;

	// Flags:
	p->SET_CARRY(result);
	p->SET_PARITY(result);
	p->SET_AUX_CARRY(x,y);
	p->SET_ZERO(result);
	p->SET_SIGN(result);

	p->REGISTERS[REG_A] = (BYTE)result;
}

void SUB(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD result, x = p->REGISTERS[REG_A], y;

	if(o1->type == OperandType::Register)
		y = p->REGISTERS[o1->value];
	else
		y = p->loadFromMemory();

	y = 256-y;
	result = x + y;

	// Flags:
	p->SET_CARRY(result);
	p->SET_PARITY(result);
	p->SET_AUX_CARRY(x,y);
	p->SET_ZERO(result);
	p->SET_SIGN(result);

	p->CY = 1-p->CY;
	p->AC = 1-p->AC;

	p->REGISTERS[REG_A] = (BYTE)result;
}

void SBB(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD result, x = p->REGISTERS[REG_A], y;

	if(o1->type == OperandType::Register)
		y = p->REGISTERS[o1->value];
	else
		y = p->loadFromMemory();

	y = 256-(y+p->CY);
	result = x + y;

	// Flags:
	p->SET_CARRY(result);
	p->SET_PARITY(result);
	p->SET_AUX_CARRY(x,y);
	p->SET_ZERO(result);
	p->SET_SIGN(result);

	p->CY = 1-p->CY;
	p->AC = 1-p->AC;

	p->REGISTERS[REG_A] = (BYTE)result;
}

void SUI(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD result, x = p->REGISTERS[REG_A], y = o1->value;

	y = 256-y;
	result = x + y;

	// Flags:
	p->SET_CARRY(result);
	p->SET_PARITY(result);
	p->SET_AUX_CARRY(x,y);
	p->SET_ZERO(result);
	p->SET_SIGN(result);

	p->CY = 1-p->CY;
	p->AC = 1-p->AC;

	p->REGISTERS[REG_A] = (BYTE)result;
}

void SBI(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD result, x = p->REGISTERS[REG_A], y = o1->value;

	y = 256-(y+p->CY);
	result = x + y;

	// Flags:
	p->SET_CARRY(result);
	p->SET_PARITY(result);
	p->SET_AUX_CARRY(x,y);
	p->SET_ZERO(result);
	p->SET_SIGN(result);

	p->CY = 1-p->CY;
	p->AC = 1-p->AC;

	p->REGISTERS[REG_A] = (BYTE)result;
}

void INR(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD result, x;

	if(o1->type == OperandType::Register)
		x = p->REGISTERS[o1->value];
	else
		x = p->loadFromMemory();

	result = x+1;

	// Flags:
	p->SET_PARITY(result);
	p->SET_AUX_CARRY(x,1);
	p->SET_ZERO(result);
	p->SET_SIGN(result);

	if(o1->type == OperandType::Register)
		p->REGISTERS[o1->value] = (BYTE)result;
	else
		p->storeToMemory((BYTE)result);
}

void INX(Operand^ o1, Operand^ o2, Processor^ p)
{
	if(o1->type == OperandType::Register) {
		if(!isRP(o1->value)) {
			p->syntaxError("INX takes only register pair or SP as operand");
			return;
		}
		WORD result = (p->REGISTERS[o1->value] << 8) + p->REGISTERS[o1->value+1] + 1;

		p->REGISTERS[o1->value] = (BYTE)(result >> 8);
		p->REGISTERS[o1->value+1] = (BYTE)(result & 0xFF);
	} else {
		p->SP++;
	}
}

void DCR(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD result, x;

	if(o1->type == OperandType::Register)
		x = p->REGISTERS[o1->value];
	else
		x = p->loadFromMemory();

	result = x-1;

	// Flags:
	p->SET_PARITY(result);
	p->SET_AUX_CARRY(x,255);
	p->SET_ZERO(result);
	p->SET_SIGN(result);

	p->AC = 1-p->AC;

	if(o1->type == OperandType::Register)
		p->REGISTERS[o1->value] = (BYTE)result;
	else
		p->storeToMemory((BYTE)result);
}

void DCX(Operand^ o1, Operand^ o2, Processor^ p)
{
	if(o1->type == OperandType::Register) {
		if(!isRP(o1->value)) {
			p->syntaxError("DCX takes only register pair or SP as operand");
			return;
		}
		WORD result = (p->REGISTERS[o1->value] << 8) + p->REGISTERS[o1->value+1] - 1;

		p->REGISTERS[o1->value] = (BYTE)(result >> 8);
		p->REGISTERS[o1->value+1] = (BYTE)(result & 0xFF);
	} else {
		p->SP--;
	}
}

void DAD(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD x;

	if(o1->type == OperandType::Register) {
		if(!isRP(o1->value)) {
			p->syntaxError("DCX takes only register pair or SP as operand");
			return;
		}

		x = (p->REGISTERS[o1->value] << 8) + p->REGISTERS[o1->value+1];
	} else {
		x = p->SP;
	}

	x = x + (p->REGISTERS[REG_H] << 8) + p->REGISTERS[REG_L];

	p->CY = BYTE(x >> 15);
	p->REGISTERS[REG_L] = BYTE(x);
	p->REGISTERS[REG_H] = BYTE(x >> 8);
}

void DAA(Operand^ o1, Operand^ o2, Processor^ p)
{
	BYTE A = p->REGISTERS[REG_A];

	if(((A & 0x0F) > 9) || (p->AC > 0)) {
		A += 0x06;
		p->AC = 1;
	} else {
		p->AC = 0;
	}
	if(((A >> 4) > 9) || (p->CY > 0)) {
		A += 0x60;
		p->CY = 1;
	} else {
		p->CY = 0;
	}

	p->SET_PARITY(A);
	p->SET_SIGN(A);
	p->SET_ZERO(A);

	p->REGISTERS[REG_A] = A;
}
/*
	          E N D  O F
	A R I T H M E T I C  G R O U P
*/

/*
						L O G I C A L  G R O U P
(ANA, ANI, XRA, XRI, ORA, ORI, CMP, CPI, RLC, RAL, RRC, RAR, CMA, CMC, STC)
*/
void ANA(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD result, x = p->REGISTERS[REG_A], y;

	if(o1->type == OperandType::Register)
		y = p->REGISTERS[o1->value];
	else
		y = p->loadFromMemory();

	result = x & y;

	// Flags:
	p->CY = 0;
	p->AC = 1;
	p->SET_PARITY(result);
	p->SET_ZERO(result);
	p->SET_SIGN(result);

	p->REGISTERS[REG_A] = (BYTE)result;
}

void ANI(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD result, x = p->REGISTERS[REG_A], y = o1->value;
	result = x & y;

	// Flags:
	p->CY = 0;
	p->AC = 1;
	p->SET_PARITY(result);
	p->SET_ZERO(result);
	p->SET_SIGN(result);

	p->REGISTERS[REG_A] = (BYTE)result;
}
void XRA(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD result, x = p->REGISTERS[REG_A], y;

	if(o1->type == OperandType::Register)
		y = p->REGISTERS[o1->value];
	else
		y = p->loadFromMemory();

	result = x ^ y;

	// Flags:
	p->CY = 0;
	p->AC = 0;
	p->SET_PARITY(result);
	p->SET_ZERO(result);
	p->SET_SIGN(result);

	p->REGISTERS[REG_A] = (BYTE)result;
}
void XRI(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD result, x = p->REGISTERS[REG_A], y = o1->value;
	result = x ^ y;

	// Flags:
	p->CY = 0;
	p->AC = 0;
	p->SET_PARITY(result);
	p->SET_ZERO(result);
	p->SET_SIGN(result);

	p->REGISTERS[REG_A] = (BYTE)result;
}
void ORA(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD result, x = p->REGISTERS[REG_A], y;

	if(o1->type == OperandType::Register)
		y = p->REGISTERS[o1->value];
	else
		y = p->loadFromMemory();

	result = x | y;

	// Flags:
	p->CY = 0;
	p->AC = 0;
	p->SET_PARITY(result);
	p->SET_ZERO(result);
	p->SET_SIGN(result);

	p->REGISTERS[REG_A] = (BYTE)result;
}
void ORI(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD result, x = p->REGISTERS[REG_A], y = o1->value;
	result = x | y;

	// Flags:
	p->CY = 0;
	p->AC = 0;
	p->SET_PARITY(result);
	p->SET_ZERO(result);
	p->SET_SIGN(result);

	p->REGISTERS[REG_A] = (BYTE)result;
}
void CMP(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD result, x = p->REGISTERS[REG_A], y;

	if(o1->type == OperandType::Register)
		y = p->REGISTERS[o1->value];
	else
		y = p->loadFromMemory();

	y = 256-y;
	result = x + y;

	// Flags:
	p->SET_CARRY(result);
	p->SET_PARITY(result);
	p->SET_AUX_CARRY(x,y);
	p->SET_ZERO(result);
	p->SET_SIGN(result);

	p->CY = 1-p->CY;
	p->AC = 1-p->AC;
}

void CPI(Operand^ o1, Operand^ o2, Processor^ p)
{
	WORD result, x = p->REGISTERS[REG_A], y = o1->value;

	y = 256-y;
	result = x + y;

	// Flags:
	p->SET_CARRY(result);
	p->SET_PARITY(result);
	p->SET_AUX_CARRY(x,y);
	p->SET_ZERO(result);
	p->SET_SIGN(result);

	p->CY = 1-p->CY;
	p->AC = 1-p->AC;
}

void RLC(Operand^ o1, Operand^ o2, Processor^ p)
{
	BYTE A = p->REGISTERS[REG_A];

	p->CY = A >> 7;

	p->REGISTERS[REG_A] = (A << 1) + p->CY;
}

void RAL(Operand^ o1, Operand^ o2, Processor^ p)
{
	BYTE A = p->REGISTERS[REG_A];

	p->REGISTERS[REG_A] = (A << 1) + p->CY;
	p->CY = A >> 7;
}

void RRC(Operand^ o1, Operand^ o2, Processor^ p)
{
	BYTE A = p->REGISTERS[REG_A];

	p->CY = A & 1;

	p->REGISTERS[REG_A] = (A >> 1) + (p->CY << 7);
}

void RAR(Operand^ o1, Operand^ o2, Processor^ p)
{
	BYTE A = p->REGISTERS[REG_A];

	p->REGISTERS[REG_A] = (A >> 1) + (p->CY << 7);

	p->CY = A & 1;
}

void CMA(Operand^ o1, Operand^ o2, Processor^ p)
{
	p->REGISTERS[REG_A] = ~p->REGISTERS[REG_A];
}

void CMC(Operand^ o1, Operand^ o2, Processor^ p)
{
	p->CY = !p->CY;
}

void STC(Operand^ o1, Operand^ o2, Processor^ p)
{
	p->CY = 1;
}
/*
	          E N D  O F
	L O G I C A L  G R O U P
*/

/*
			C O N T R O L  T R A N S F E R  G R O U P
	(JMP, JNZ, JZ, JNC, JC, JPO, JPE, JP, JM, 
	CALL, CNZ, CZ, CNC, CC, CPO, CPE, CP, CM,
	 RET, RNZ, RZ, RNC, RC, RPO, RPE, RP, RM)
*/
void JMP(Operand^ o1, Operand^ o2, Processor^ p)
{
	p->jumpTo(o1->value);
}

void JNZ(Operand^ o1, Operand^ o2, Processor^ p)
{
	if(!p->Z)
		p->jumpTo(o1->value);
	else
		p->jumpTo(p->PC+3);
}

void JZ(Operand^ o1, Operand^ o2, Processor^ p)
{
	if(p->Z)
		p->jumpTo(o1->value);
	else
		p->jumpTo(p->PC+3);
}

void JNC(Operand^ o1, Operand^ o2, Processor^ p)
{
	if(!p->CY)
		p->jumpTo(o1->value);
	else
		p->jumpTo(p->PC+3);
}

void JC(Operand^ o1, Operand^ o2, Processor^ p)
{
	if(p->CY)
		p->jumpTo(o1->value);
	else
		p->jumpTo(p->PC+3);
}

void JPO(Operand^ o1, Operand^ o2, Processor^ p)
{
	if(!p->P)
		p->jumpTo(o1->value);
	else
		p->jumpTo(p->PC+3);
}

void JPE(Operand^ o1, Operand^ o2, Processor^ p)
{
	if(p->P)
		p->jumpTo(o1->value);
	else
		p->jumpTo(p->PC+3);
}


void JP(Operand^ o1, Operand^ o2, Processor^ p)
{
	if(!p->S)
		p->jumpTo(o1->value);
	else
		p->jumpTo(p->PC+3);
}

void JM(Operand^ o1, Operand^ o2, Processor^ p)
{
	if(p->P)
		p->jumpTo(o1->value);
	else
		p->jumpTo(p->PC+3);
}
/*
	             E N D  O F
	C O N T R O L  T R A N S F E R  G R O U P
*/
/*
	S T A C K  C O N T R O L  G R O U P
		  (PUSH, POP, XTHL, SPHL)
*/
void PUSH(Operand^ o1, Operand^ o2, Processor^ p)
{
	if(o1->type == OperandType::Register && !isRP(o1->value)) {
		p->syntaxError("PUSH expects only Register Pairs [B/D/H]");
		return;
	}

	WORD x;
	if(o1->type == OperandType::Register) {
		x = (p->REGISTERS[o1->value] << 8) + p->REGISTERS[o1->value+1];
	} else {
		x = p->CY + (p->P << 2) + (p->AC << 4) + (p->Z << 6) + (p->S << 7) + ((WORD)p->REGISTERS[REG_A] << 8);
	}
	p->push(x);
}
void POP(Operand^ o1, Operand^ o2, Processor^ p)
{
	if(o1->type == OperandType::Register && !isRP(o1->value)) {
		p->syntaxError("POP expects only Register Pairs [B/D/H]");
		return;
	}

	WORD x = p->pop();

	if(o1->type == OperandType::Register) {
		p->REGISTERS[o1->value] = (BYTE)(x >> 8);
		p->REGISTERS[o1->value+1] = (BYTE)x;
	} else {
		p->REGISTERS[REG_A] = (BYTE)(x >> 8);
		p->CY = x & 1;
		p->P = (x >> 2) & 1;
		p->AC = (x >> 4) & 1;
		p->Z = (x >> 6) & 1;
		p->S = (x >> 7) & 1;
	}
}
/*
	             E N D  O F
	S T A C K  C O N T R O L  G R O U P
*/
/*
	M A C H I N E  C O N T R O L  G R O U P
*/
void HLT(Operand^ o1, Operand^ o2, Processor^ p)
{
	p->HALTED = 1;
}

void Processor::initializeOperations()
{
	OperandType R = OperandType::Register;
	OperandType M = OperandType::Memory;
	OperandType RorM = R | M;
	OperandType I = OperandType::Immediate;
	OperandType A = OperandType::Address;
	OperandType RorSP = R | OperandType::StackPointer;
	OperandType RorPSW = R | OperandType::PSW;

	// Data Transfer group:
	Operation Move(MOV, OperationType::DATA_TRANSFER, RorM, RorM);
	Operation MoveImmediate(MVI, OperationType::DATA_TRANSFER, RorM, I);
	Operation LoadADirect(LDA, OperationType::DATA_TRANSFER, A);
	Operation StoreADirect(STA, OperationType::DATA_TRANSFER, A);
	Operation LoadHLDirect(LHLD, OperationType::DATA_TRANSFER, A);
	Operation StoreHLDirect(SHLD, OperationType::DATA_TRANSFER, A);
	Operation ExchangeHLDE(XCHG, OperationType::DATA_TRANSFER);
	Operation LoadRPImmediate(LXI, OperationType::DATA_TRANSFER, RorSP, A);
	Operation LoadAIndirect(LDAX, OperationType::DATA_TRANSFER, R);
	Operation StoreAIndirect(STAX, OperationType::DATA_TRANSFER, R);

	parser.addOperation("MOV", Move);
	parser.addOperation("MVI", MoveImmediate);
	parser.addOperation("LDA", LoadADirect);
	parser.addOperation("STA", StoreADirect);
	parser.addOperation("LHLD", LoadHLDirect);
	parser.addOperation("SHLD", StoreHLDirect);
	parser.addOperation("XCHG", ExchangeHLDE);
	parser.addOperation("LXI", LoadRPImmediate);
	parser.addOperation("LDAX", LoadAIndirect);
	parser.addOperation("STAX", StoreAIndirect);

	// Arithmetic group:
	Operation Add(ADD, OperationType::ARITHMETIC, RorM);
	Operation AddCarry(ADC, OperationType::ARITHMETIC, RorM);
	Operation AddImmediate(ADI, OperationType::ARITHMETIC, I);
	Operation AddImmediateCarry(ACI, OperationType::ARITHMETIC, I);
	Operation Sub(SUB, OperationType::ARITHMETIC, RorM);
	Operation SubBorrow(SBB, OperationType::ARITHMETIC, RorM);
	Operation SubImmediate(SUI, OperationType::ARITHMETIC, I);
	Operation SubImmediateBorrow(SBI, OperationType::ARITHMETIC, I);
	Operation Increment(INR, OperationType::ARITHMETIC, RorM);
	Operation IncrementRP(INX, OperationType::ARITHMETIC, RorSP);
	Operation Decrement(DCR, OperationType::ARITHMETIC, RorM);
	Operation DecrementRP(DCX, OperationType::ARITHMETIC, RorSP);
	Operation DoubleAdd(DAD, OperationType::ARITHMETIC, RorSP);
	Operation DecimalAdjustA(DAA, OperationType::ARITHMETIC);

	parser.addOperation("ADD", Add);
	parser.addOperation("ADC", AddCarry);
	parser.addOperation("ADI", AddImmediate);
	parser.addOperation("ACI", AddImmediateCarry);
	parser.addOperation("SUB", Sub);
	parser.addOperation("SBB", SubBorrow);
	parser.addOperation("SUI", SubImmediate);
	parser.addOperation("SBI", SubImmediateBorrow);
	parser.addOperation("INR", Increment);
	parser.addOperation("INX", IncrementRP);
	parser.addOperation("DCR", Decrement);
	parser.addOperation("DCX", DecrementRP);
	parser.addOperation("DAD", DoubleAdd);
	parser.addOperation("DAA", DecimalAdjustA);

	// Logical group:
	Operation And(ANA, OperationType::LOGICAL, RorM);
	Operation AndImmediate(ANI, OperationType::LOGICAL, I);
	Operation Xor(XRA, OperationType::LOGICAL, RorM);
	Operation XorImmediate(XRI, OperationType::LOGICAL, I);
	Operation Or(ORA, OperationType::LOGICAL, RorM);
	Operation OrImmediate(ORI, OperationType::LOGICAL, I);
	Operation Compare(CMP, OperationType::LOGICAL, RorM);
	Operation CompareImmediate(CPI, OperationType::LOGICAL, I);
	Operation RotateLeft(RLC, OperationType::LOGICAL);
	Operation RotateLeftCarry(RAL, OperationType::LOGICAL);
	Operation RotateRight(RRC, OperationType::LOGICAL);
	Operation RotateRightCarry(RAR, OperationType::LOGICAL);
	Operation ComplementA(CMA, OperationType::LOGICAL);
	Operation ComplementCarry(CMC, OperationType::LOGICAL);
	Operation SetCarry(STC, OperationType::LOGICAL);

	parser.addOperation("ANA", And);
	parser.addOperation("ANI", AndImmediate);
	parser.addOperation("XRA", Xor);
	parser.addOperation("XRI", XorImmediate);
	parser.addOperation("ORA", Or);
	parser.addOperation("ORI", OrImmediate);
	parser.addOperation("CMP", Compare);
	parser.addOperation("CPI", CompareImmediate);
	parser.addOperation("RLC", RotateLeft);
	parser.addOperation("RAL", RotateLeftCarry);
	parser.addOperation("RRC", RotateRight);
	parser.addOperation("RAR", RotateRightCarry);
	parser.addOperation("CMA", ComplementA);
	parser.addOperation("CMC", ComplementCarry);
	parser.addOperation("STC", SetCarry);

	// Control Transfer Group:
	Operation Jump(JMP, OperationType::CONTROL_TRANSFER, A);
	Operation JumpNoZero(JNZ, OperationType::CONTROL_TRANSFER, A);
	Operation JumpZero(JZ, OperationType::CONTROL_TRANSFER, A);
	Operation JumpNoCarry(JNC, OperationType::CONTROL_TRANSFER, A);
	Operation JumpCarry(JC, OperationType::CONTROL_TRANSFER, A);
	Operation JumpParityOdd(JPO, OperationType::CONTROL_TRANSFER, A);
	Operation JumpParityEven(JPE, OperationType::CONTROL_TRANSFER, A);
	Operation JumpPlus(JP, OperationType::CONTROL_TRANSFER, A);
	Operation JumpMinus(JM, OperationType::CONTROL_TRANSFER, A);

	parser.addOperation("JMP", Jump);
	parser.addOperation("JNZ", JumpNoZero);
	parser.addOperation("JZ", JumpZero);
	parser.addOperation("JNC", JumpNoCarry);
	parser.addOperation("JC", JumpCarry);
	parser.addOperation("JPO", JumpParityOdd);
	parser.addOperation("JPE", JumpParityEven);
	parser.addOperation("JP", JumpPlus);
	parser.addOperation("JM", JumpMinus);

	// Stack Control Group:
	Operation Push(PUSH, OperationType::STACK_CONTROL, RorPSW);
	Operation Pop(POP, OperationType::STACK_CONTROL, RorPSW);

	parser.addOperation("PUSH", Push);
	parser.addOperation("POP", Pop);

	// Machine Control Group:
	Operation Halt(HLT, OperationType::MACHINE_CONTROL);

	parser.addOperation("HLT", Halt);
}

void Processor::syntaxError(char* error) {
	//printf("SYNTAX ERROR: %s\n", error);
	String^ s = gcnew String(error);
	throw gcnew ArgumentException("Syntax Error: " + s);
}

/*
	Flag modification routines
*/
void Processor::SET_CARRY(WORD x) {
	CY = (x >> 8);
}
void Processor::SET_ZERO(WORD x) {
	Z = ((char)x == 0);
}
void Processor::SET_SIGN(WORD x) {
	S = ((x >> 7) & 1);
}
void Processor::SET_PARITY(WORD result) {
	char parity = 0;

	for(int i = 0; i < 8; i++)
		parity += (result >> i) & 1;

	P = !(parity & 1);
}
void Processor::SET_AUX_CARRY(WORD o1, WORD o2) {
	AC = ((o1 & 0x0F) + (o2 & 0x0F)) >> 4;
}
void Processor::jumpTo(WORD pc) {
	// Exception will be thrown if no instruction is found:
	instructions->getInstructionAt(pc);
	PC = pc;
}