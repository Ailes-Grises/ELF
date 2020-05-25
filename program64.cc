#include<iostream>
#include<string>
#include<string.h>
#include<elf.h>
#include"elf64.h"

Program::Program(Device &bd, Elf &eh){
	phdr=new Elf64_Phdr[eh.E_phnum()];
	for(int i=0;i<eh.E_phnum();i++) memset(&phdr[i], 0, sizeof(phdr[i]));
	this->ph_parser(bd, eh);
};

Program::~Program(void){
	delete [] phdr;
};

void Program::ph_parser(Device &bd, Elf &eh){

	//Device クラスのデータカウンタの位置をe_phoff に移動する．
	bd.setDC(eh.E_phoff());

	for(int i=0;i<eh.E_phnum();i++){
		// 仕様書に従って構造体へダンプ
		phdr[i].p_type=bd.get32bit();
		phdr[i].p_flags=bd.get32bit();
		phdr[i].p_offset=bd.get64bit();
		phdr[i].p_vaddr=bd.get64bit();
		phdr[i].p_paddr=bd.get64bit();
		phdr[i].p_filesz=bd.get64bit();
		phdr[i].p_memsz=bd.get64bit();
		phdr[i].p_align=bd.get64bit();
	}
};

void Program::show_phdr(Device &bd, Elf &eh){

	cout<<"This ELF file is of type DYN (Shared Object File). "<<endl;
	cout<<"Entry Point : 0x"<<hexformat(8)<<eh.E_entry()<<endl;
	cout<<"There are "<<eh.E_phnum()<<" program headers, starting at offset 0x"<<hexformat(8)<<eh.E_phoff()<<"\n"<<endl;

	cout<<"Program Header: "<<endl;
	cout<<setw_left(18)<<"  Type";
	cout<<setw_left(20)<<"Offset";
	cout<<setw_left(19)<<"Virtual Addr";
	cout<<"  Physical Addr"<<endl;
	cout<<setw_right(29)<<"File Size  ";
	cout<<setw_right(20)<<"Memory Size";
	cout<<setw_right(14)<<"Flags";
	cout<<"  Alignment\n"<<endl;

	for(int i=0;i<eh.E_phnum();i++){
		/* Type */
		switch(phdr[i].p_type){
			case PT_NULL:
				cout<<setw_left(18)<<"  NULL"; break;
			case PT_LOAD:
				cout<<setw_left(18)<<"  LOAD"; break;
			case PT_DYNAMIC:
				cout<<setw_left(18)<<"  DYNAMIC"; break;
			case PT_INTERP:
				cout<<setw_left(18)<<"  INTERP"; break;
			case PT_NOTE:
				cout<<setw_left(18)<<"  NOTE"; break;
			case PT_SHLIB:
				cout<<setw_left(18)<<"  SHLIB"; break;
			case PT_PHDR:
				cout<<setw_left(18)<<"  PHDR"; break;
			case PT_TLS:
				cout<<setw_left(18)<<"  TLS"; break;
			case PT_NUM:
				cout<<setw_left(18)<<"  NUM"; break;
			case PT_LOOS:
				cout<<setw_left(18)<<"  LOOS"; break;
			case PT_GNU_EH_FRAME:
				cout<<setw_left(18)<<"  GNU_EH_FRAME"; break;
			case PT_GNU_STACK:
				cout<<setw_left(18)<<"  GNU_STACK"; break;
			case PT_GNU_RELRO:
				cout<<setw_left(18)<<"  GNU_RELRO"; break;
			case PT_LOSUNW:
				cout<<setw_left(18)<<"  LOSUNW (or SUNWBSS)"; break;
			// Value Confliction
			// case PT_SUNWBSS:
			// 	cout<<setw_left(18)<  <"SUNWBSS"; break;
			case PT_SUNWSTACK:
				cout<<setw_left(18)<<"  SUNWSTACK"; break;
			case PT_HISUNW:
				cout<<setw_left(18)<<"  HISUNW (or HIOS)"; break;
			// case PT_HIOS:
			// 	cout<<setw_left(18)<  <"HIOS"; break;
			case PT_LOPROC:
				cout<<setw_left(18)<<"  LOPROC"; break;
			case PT_HIPROC:
				cout<<setw_left(18)<<"  HIPROC"; break;
			default:
				cout<<setw_left(18)<<"  UNKNOWN"; break;
		}
		cout<<std::right;

		/* Offset */
		cout<<"0x"<<hexformat(16)<<phdr[i].p_offset;

		/* Virtual Address */
		cout<<"  0x"<<hexformat(16)<<phdr[i].p_vaddr;

		/* Physical Address */
		cout<<"  0x"<<hexformat(16)<<phdr[i].p_paddr<<endl;;

		/* File Size */
		cout<<"                  0x"<<hexformat(16)<<phdr[i].p_filesz;

		/* Memory Size */
		cout<<"  0x"<<hexformat(16)<<phdr[i].p_memsz;

		/* Flags */
		std::string flags;
		if((phdr[i].p_flags & PF_X)==PF_X) flags.insert(flags.begin(),'X');
		if((phdr[i].p_flags & PF_W)==PF_W) flags.insert(flags.begin(),'W');
		if((phdr[i].p_flags & PF_R)==PF_R) flags.insert(flags.begin(),'R');
		if((phdr[i].p_flags & PF_MASKOS)==PF_MASKOS) flags+='O';
		if((phdr[i].p_flags & PF_MASKPROC)==PF_MASKPROC) flags+='P';

		cout<<"   "<<setw_left(4)<<flags<<std::right;

		/* Alignment */
		cout<<"   0x"<<hexformat(8)<<phdr[i].p_align<<"\n"<<endl;;

	}

};
