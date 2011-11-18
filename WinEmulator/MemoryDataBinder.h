#pragma once
#include "Processor.h"

using namespace System;
using namespace System::Windows::Forms;

ref class MemoryDataBinder {
private:
	WORD address;
	Processor^ processor;

	String^ getAt(int index) {
		return Convert::ToString(processor->memory[address+index],16)->ToUpper()->PadLeft(2,'0');
	}
	void setAt(String^ s, int index) {
		try {
			processor->memory[address+index] = Convert::ToByte(s, 16);
		}
		catch(Exception^) {
			MessageBox::Show("Invalid immediate data.");
		}
	}
public:
	MemoryDataBinder(WORD a, Processor^ p) {
		address = a;
		processor = p;
	}
	property String^ Address {
		String^ get() {return Convert::ToString(address,16)->ToUpper()->PadLeft(4, '0');}
	}
	property String^ Zero {
		String^ get() {	return getAt(0); }
		void set(String^ s) { setAt(s, 0); }
	}
	property String^ One {
		String^ get() {	return getAt(1); }
		void set(String^ s) { setAt(s, 1); }
	}
	property String^ Two {
		String^ get() {	return getAt(2); }
		void set(String^ s) { setAt(s, 2); }
	}
	property String^ Three {
		String^ get() {	return getAt(3); }
		void set(String^ s) { setAt(s, 3); }
	}
	property String^ Four {
		String^ get() {	return getAt(4); }
		void set(String^ s) { setAt(s, 4); }
	}
	property String^ Five {
		String^ get() {	return getAt(5); }
		void set(String^ s) { setAt(s, 5); }
	}
	property String^ Six {
		String^ get() {	return getAt(6); }
		void set(String^ s) { setAt(s, 6); }
	}
	property String^ Seven {
		String^ get() {	return getAt(7); }
		void set(String^ s) { setAt(s, 7); }
	}
	property String^ Eight {
		String^ get() {	return getAt(8); }
		void set(String^ s) { setAt(s, 8); }
	}
	property String^ Nine {
		String^ get() {	return getAt(9); }
		void set(String^ s) { setAt(s, 9); }
	}
	property String^ Ten {
		String^ get() {	return getAt(10); }
		void set(String^ s) { setAt(s, 10); }
	}
	property String^ Eleven {
		String^ get() {	return getAt(11); }
		void set(String^ s) { setAt(s, 11); }
	}
	property String^ Twelve {
		String^ get() {	return getAt(12); }
		void set(String^ s) { setAt(s, 12); }
	}
	property String^ Thirteen {
		String^ get() {	return getAt(13); }
		void set(String^ s) { setAt(s, 13); }
	}
	property String^ Fourteen {
		String^ get() {	return getAt(14); }
		void set(String^ s) { setAt(s, 14); }
	}
	property String^ Fifteen {
		String^ get() {	return getAt(15); }
		void set(String^ s) { setAt(s, 15); }
	}
};
