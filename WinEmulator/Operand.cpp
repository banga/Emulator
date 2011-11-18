#include "StdAfx.h"
#include "Operand.h"

Operand::Operand(const Operand% o) {
	type = o.type;
	value = o.value;
}

Operand::Operand(OperandType t, WORD v) {
	type = t;
	value = v;
}