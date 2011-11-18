#pragma once
#include "Processor.h"

using namespace System;
using namespace System::Windows::Forms;

ref class RegisterDataBinder {
private:
	String^ _name;
	Processor^ processor;

public:
	RegisterDataBinder(String^ n, Processor^ p) {
		_name = n;
		processor = p;
	}
	property String^ Name {
		String^ get() {
			return _name;
		}
		void set(String^ name) {
			if(name->Length == 1 && Executor::charToRegisterIndex(name[0]) != -1) {
				_name = name;
			} else if(name == "BC" || name == "DE" || name == "HL" || name == "PC" || name == "SP") {
				_name = name;
			} else {
				MessageBox::Show("No such register found");
			}
		}
	}
	property String^ Value {
		///TODO: Add support for other registers
		String^ get() {
			int index;

			String^ s;

			if(_name->Length == 1 && (index = Executor::charToRegisterIndex(_name[0])) != -1) {
				s = Convert::ToString(processor->REGISTERS[index], 16);
				s = s->PadLeft(2, '0');
			} else if(_name == "BC" || _name == "DE" || _name == "HL") {
				index = Executor::charToRegisterIndex(_name[0]);
				s = Convert::ToString((processor->REGISTERS[index]<<8) + processor->REGISTERS[index+1], 16);
				s = s->PadLeft(4, '0');
			} else if(_name == "PC") {
				s = Convert::ToString(processor->PC, 16);
				s = s->PadLeft(4, '0');
			} else if(_name == "SP") {
				s = Convert::ToString(processor->SP, 16);
				s = s->PadLeft(4, '0');
			} else {
				s = "Undefined";
			}

			return s->ToUpper();
		}
		void set(String^ s) {
			try {
				int index;

				if(_name->Length == 1 && (index = Executor::charToRegisterIndex(_name[0])) != -1) {
					processor->REGISTERS[index] = Convert::ToByte(s, 16);
				} else if(_name == "BC" || _name == "DE" || _name == "HL") {
					index = Executor::charToRegisterIndex(_name[0]);
					int value = Convert::ToInt16(s, 16);
					processor->REGISTERS[index] = (BYTE)(value >> 8);
					processor->REGISTERS[index+1] = (BYTE)value;
				} else if(_name == "PC") {
					processor->PC = Convert::ToInt16(s, 16);
				} else if(_name == "SP") {
					processor->SP = Convert::ToInt16(s, 16);
				}
			}
			catch(Exception^) {
				MessageBox::Show("Couldn't convert " + s + " to immediate value", "Conversion Error");
			}
		}
	}
};
