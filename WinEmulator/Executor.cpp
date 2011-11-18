#include "StdAfx.h"
#include "Executor.h"

int Executor::charToRegisterIndex(Char c)
{
	return Executor::RegisterNames->IndexOf(c);
}

void Executor::execute(Instruction^ i, Processor^ p)
{
	if(!i) throw gcnew NullReferenceException("Argument 'Instruction^' is null");
	if(!i->operation) throw gcnew NullReferenceException("Argument 'Instruction^' has operation^ == null");
	if(!i->operation->fptr) throw gcnew NullReferenceException("Delegate fptr is null");

	i->operation->fptr(i->op1, i->op2, p);
}