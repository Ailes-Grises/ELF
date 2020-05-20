#include<iostream>
#include<elf.h>
#include"elf64.h"

Symbol::Symbol(void){
};

Symbol::Symbol(Device &bd, int sym_type){
	Symbol::sym_type=sym_type;
	// sym_type の値によって確保する領域サイズを変更する．
	switch(Symbol::sym_type){
		case DYNSYM:
			// .dynsym と同じサイズだけ確保
			// symbol = new Elf64_Sym[]; break;
		case SYMTAB:
			// .symbol と同じサイズだけ確保
			// symbol = new Elf64_Sym[]; break;
		default:
			std::cerr<<"\x1b[33m[Error] Invalid symbol type. (Symbol::Symbol(Device &bd, int sym_type))\x1b[0m"<<std::endl;
			break;
	}
};

Symbol::~Symbol(void){
	delete [] symbol;
};

void Symbol::sym_parser(Device &bd, Section &sh){
	switch(sym_type){
		case DYNSYM:
			break;
		case SYMTAB:
			break;
		default:
			break;
	}
};

void Symbol::show_symtab(device &bd, Section &sh){
};
