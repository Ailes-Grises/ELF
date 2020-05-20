#include<iostream>
#include<string>
#include<elf.h>
#include"elf64.h"

Program::Program(void){
};

Program::Program(Device &bd, Elf &eh){
	phdr=new Elf64_Phdr[eh.E_phnum()];
	this->ph_parser(bd, eh);
};

Program::~Program(void){
	delete [] phdr;
};

void Program::ph_parser(Device &bd, Elf &eh){

	//Device クラスのデータカウンタの位置をe_phoff に移動する．
	bd.setDC(bd, eh.E_phoff());

	for(int i=0;i<eh.E_phnum();i++){
		// 仕様書に従って構造体へダンプ
		phdr[i].p_type=bd.get32bit(bd);
		phdr[i].p_flags=bd.get32bit(bd);
		phdr[i].p_offset=bd.get64bit(bd);
		phdr[i].p_vaddr=bd.get64bit(bd);
		phdr[i].p_paddr=bd.get64bit(bd);
		phdr[i].p_filesz=bd.get64bit(bd);
		phdr[i].p_memsz=bd.get64bit(bd);
		phdr[i].p_align=bd.get64bit(bd);
	}
};

void Program::show_phdr(Device &bd, Elf &eh){

	std::cout<<"This ELF file is of type DYN (Shared Object File). "<<std::endl;
	std::cout<<"Entry Point : 0x"<<hexformat(8)<<eh.E_entry()<<std::endl;
	std::cout<<"There are "<<eh.E_phnum()<<" program headers, starting at offset 0x"<<hexformat(8)<<eh.E_phoff()<<"\n"<<std::endl;

	std::cout<<"Program Header: "<<std::endl;
	std::cout<<setw_left(18)<<"  Type"<<setw_left(20)<<"Offset"<<setw_left(19)<<"Virtual Addr"<<"  Physical Addr"<<std::endl;
	std::cout<<setw_right(29)<<"File Size  "<<setw_right(20)<<"Memory Size"<<setw_right(14)<<"Flags"<<"  Alignment\n"<<std::endl;

	for(int i=0;i<eh.E_phnum();i++){
		/* Type */
		switch(phdr[i].p_type){
			case PT_NULL:
				std::cout<<setw_left(18)<<"  NULL"; break;
			case PT_LOAD:
				std::cout<<setw_left(18)<<"  LOAD"; break;
			case PT_DYNAMIC:
				std::cout<<setw_left(18)<<"  DYNAMIC"; break;
			case PT_INTERP:
				std::cout<<setw_left(18)<<"  INTERP"; break;
			case PT_NOTE:
				std::cout<<setw_left(18)<<"  NOTE"; break;
			case PT_SHLIB:
				std::cout<<setw_left(18)<<"  SHLIB"; break;
			case PT_PHDR:
				std::cout<<setw_left(18)<<"  PHDR"; break;
			case PT_TLS:
				std::cout<<setw_left(18)<<"  TLS"; break;
			case PT_NUM:
				std::cout<<setw_left(18)<<"  NUM"; break;
			case PT_LOOS:
				std::cout<<setw_left(18)<<"  LOOS"; break;
			case PT_GNU_EH_FRAME:
				std::cout<<setw_left(18)<<"  GNU_EH_FRAME"; break;
			case PT_GNU_STACK:
				std::cout<<setw_left(18)<<"  GNU_STACK"; break;
			case PT_GNU_RELRO:
				std::cout<<setw_left(18)<<"  GNU_RELRO"; break;
			case PT_LOSUNW:
				std::cout<<setw_left(18)<<"  LOSUNW (or SUNWBSS)"; break;
			// Value Confliction
			// case PT_SUNWBSS:
			// 	std::cout<<setw_left(18)<  <"SUNWBSS"; break;
			case PT_SUNWSTACK:
				std::cout<<setw_left(18)<<"  SUNWSTACK"; break;
			case PT_HISUNW:
				std::cout<<setw_left(18)<<"  HISUNW (or HIOS)"; break;
			// case PT_HIOS:
			// 	std::cout<<setw_left(18)<  <"HIOS"; break;
			case PT_LOPROC:
				std::cout<<setw_left(18)<<"  LOPROC"; break;
			case PT_HIPROC:
				std::cout<<setw_left(18)<<"  HIPROC"; break;
			default:
				std::cout<<setw_left(18)<<"  UNKNOWN"; break;
		}
		std::cout<<std::right;

		/* Offset */
		std::cout<<"0x"<<hexformat(16)<<phdr[i].p_offset;

		/* Virtual Address */
		std::cout<<"  0x"<<hexformat(16)<<phdr[i].p_vaddr;

		/* Physical Address */
		std::cout<<"  0x"<<hexformat(16)<<phdr[i].p_paddr<<std::endl;;

		/* File Size */
		std::cout<<"                  0x"<<hexformat(16)<<phdr[i].p_filesz;

		/* Memory Size */
		std::cout<<"  0x"<<hexformat(16)<<phdr[i].p_memsz;

		/* Flags */
		std::string flags;
		if((phdr[i].p_flags & PF_X)==PF_X) flags.insert(flags.begin(),'X');
		if((phdr[i].p_flags & PF_W)==PF_W) flags.insert(flags.begin(),'W');
		if((phdr[i].p_flags & PF_R)==PF_R) flags.insert(flags.begin(),'R');
		if((phdr[i].p_flags & PF_MASKOS)==PF_MASKOS) flags+='O';
		if((phdr[i].p_flags & PF_MASKPROC)==PF_MASKPROC) flags+='P';

		std::cout<<"   "<<setw_left(4)<<flags<<std::right;

		/* Alignment */
		std::cout<<"   0x"<<hexformat(8)<<phdr[i].p_align<<"\n"<<std::endl;;

	}

};
