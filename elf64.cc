#include<iostream>
#include<iomanip>
#include<string.h>
#include<elf.h>
#include"elf64.h"

Elf::Elf(Device &bd){
	memset(&ehdr, 0, sizeof(ehdr));
	this->eh_parser(bd); // パーサの呼び出し
};

Elf::~Elf(void){
};

unsigned char Elf::E_ident(int Nident){
	return ehdr.e_ident[Nident];
};

uint16_t Elf::E_type(void){
	return ehdr.e_type;
};

uint16_t Elf::E_machine(void){
	return ehdr.e_machine;
};

uint32_t Elf::E_version(void){
	return ehdr.e_version;
};

Elf64_Addr Elf::E_entry(void){
	return ehdr.e_entry;
};

Elf64_Off Elf::E_phoff(void){
	return ehdr.e_phoff;
};

Elf64_Off Elf::E_shoff(void){
	return ehdr.e_shoff;
};

uint32_t Elf::E_flags(void){
	return ehdr.e_flags;
};

uint16_t Elf::E_ehsize(void){
	return ehdr.e_ehsize;
};

uint16_t Elf::E_phentsize(void){
	return ehdr.e_phentsize;
};

uint16_t Elf::E_phnum(void){
	return ehdr.e_phnum;
};

uint16_t Elf::E_shentsize(void){
	return ehdr.e_shentsize;
};

uint16_t Elf::E_shnum(void){
	return ehdr.e_shnum;
};

uint16_t Elf::E_shstrndx(void){
	return ehdr.e_shstrndx;
};

void Elf::eh_parser(Device &bd){
	for(int i=0;i<EI_NIDENT;i++) ehdr.e_ident[i]=bd.get8bit();

	ehdr.e_type=bd.get16bit();
	ehdr.e_machine=bd.get16bit();
	ehdr.e_version=bd.get32bit();
	ehdr.e_entry=bd.get64bit();
	ehdr.e_phoff=bd.get64bit();
	ehdr.e_shoff=bd.get64bit();
	ehdr.e_flags=bd.get32bit();
	ehdr.e_ehsize=bd.get16bit();
	ehdr.e_phentsize=bd.get16bit();
	ehdr.e_phnum=bd.get16bit();
	ehdr.e_shentsize=bd.get16bit();
	ehdr.e_shnum=bd.get16bit();
	ehdr.e_shstrndx=bd.get16bit();
};

void Elf::show_ehdr(void){
	cout<<"ELF Header:"<<endl;

	/* e_ident[EI_NIDENT] */

	cout<<" Magic:    ";
	for(int i=0;i<EI_NIDENT;i++) cout<<hexformat(2)<<(unsigned int)ehdr.e_ident[i]<<" ";
	cout<<endl;

	switch(ehdr.e_ident[EI_CLASS]){
		case ELFCLASS64:
			cout<<" Class:                             ELF64"<<endl; break;
		case ELFCLASS32:
			cout<<" Class:                             ELF32"<<endl; break;
		default:
			cout<<" Class:                             Invalid Class"<<endl; break;
	}

	switch(ehdr.e_ident[EI_DATA]){
		case ELFDATA2LSB:
			cout<<" Data:                              2's Complement, Little Endian"<<endl; break;
		case ELFDATA2MSB:
			cout<<" Data:                              2's Complement, Big Endian"<<endl; break;
		default:
			cout<<" Data:                              Invalid Data Format"<<endl; break;
	}

	switch(ehdr.e_ident[EI_VERSION]){
		case EV_CURRENT:
			cout<<" Version:                           1 (current)"<<endl; break;
		default:
			cout<<" Version:                           0 (illegal)"<<endl; break;
	}

	switch(ehdr.e_ident[EI_OSABI]){
		case ELFOSABI_SYSV:
			cout<<" OS/ABI:                            UNIX - System V"<<endl; break;
		case ELFOSABI_HPUX:
			cout<<" OS/ABI:                            HP-UX"<<endl; break;
		case ELFOSABI_NETBSD:
			cout<<" OS/ABI:                            NetBSD"<<endl; break;
		case ELFOSABI_LINUX:
			cout<<" OS/ABI:                            Linux"<<endl; break;
		case ELFOSABI_SOLARIS:
			cout<<" OS/ABI:                            Solaris"<<endl; break;
		case ELFOSABI_IRIX:
			cout<<" OS/ABI:                            IRIX"<<endl; break;
		case ELFOSABI_FREEBSD:
			cout<<" OS/ABI:                            FreeBSD"<<endl; break;
		case ELFOSABI_TRU64:
			cout<<" OS/ABI:                            TRU64 - UNIX"<<endl; break;
		case ELFOSABI_ARM:
			cout<<" OS/ABI:                            ARM"<<endl; break;
		case ELFOSABI_STANDALONE:
			cout<<" OS/ABI:                            Standalone"<<endl; break;
		default:
			cout<<" OS/ABI:                            UNIX - System V"<<endl; break;
	}

	// ここだけ決め打ち
	cout<<" ABI Version:                       0"<<endl;

	/* e_type */

	switch(ehdr.e_type){
		case ET_REL: 
			cout<<" Type:                              REL (Relocatable File)"<<endl; break;
		case ET_EXEC: 
			cout<<" Type:                              EXEC (Executable File)"<<endl; break;
		case ET_DYN: 
			cout<<" Type:                              DYN (Shared Object File)"<<endl; break;
		case ET_CORE: 
			cout<<" Type:                              CORE (Core File)"<<endl; break;
		default: 
			cout<<" Type:                              NONE (Invalid Type)"<<endl; break;
	}

	/* e_machine */

	switch(ehdr.e_machine){
		case EM_M32: 
			cout<<" Architecture:                      AT&T WE 32100"<<endl; break;
		case EM_SPARC: 
			cout<<" Architecture:                      Sun Microsystems SPARC"<<endl; break;
		case EM_386: 
			cout<<" Architecture:                      Intel 80386"<<endl; break;
		case EM_68K: 
			cout<<" Architecture:                      Motorola 68000"<<endl; break;
		case EM_88K: 
			cout<<" Architecture:                      Motorola 88000"<<endl; break;
		case EM_860: 
			cout<<" Architecture:                      Intel 80860"<<endl; break;
		case EM_MIPS: 
			cout<<" Architecture:                      MIPS RS3000"<<endl; break;
		case EM_PARISC: 
			cout<<" Architecture:                      HP/PA"<<endl; break;
		case EM_SPARC32PLUS: 
			cout<<" Architecture:                      SPARC with Extended Instruction Set"<<endl; break;
		case EM_PPC: 
			cout<<" Architecture:                      PowerPC"<<endl; break;
		case EM_PPC64: 
			cout<<" Architecture:                      PowerPC 64-bit"<<endl; break;
		case EM_S390: 
			cout<<" Architecture:                      IBM S/390"<<endl; break;
		case EM_ARM: 
			cout<<" Architecture:                      ARM"<<endl; break;
		case EM_SH: 
			cout<<" Architecture:                      Renesas SuperH"<<endl; break;
		case EM_SPARCV9: 
			cout<<" Architecture:                      SPARC v9 64-bit"<<endl; break;
		case EM_IA_64: 
			cout<<" Architecture:                      Intel Itanium"<<endl; break;
		case EM_X86_64: 
			cout<<" Architecture:                      AMD x86-64"<<endl; break;
		case EM_VAX: 
			cout<<" Architecture:                      DEC Vax"<<endl; break;
		default: 
			cout<<" Architecture:                      Unknown Architecture"<<endl; break;
	}

	/* e_version */

	switch(ehdr.e_version){
		case EV_CURRENT:
			cout<<" Version:                           Current Version(0x01)"<<endl; break;
		default:
			cout<<" Version:                           Unknown Version(0x00)"<<endl; break;
	}

	/* e_entry */

	cout<<" Entry Point Address:               0x"<<hexformat(8)<<ehdr.e_entry<<endl;

	/* e_flags */

	cout<<" Flag:                              0x"<<hexformat(2)<<ehdr.e_flags<<endl;

	/* e_ehsize */

	cout<<" Elf Header Size:                   "<<std::dec<<ehdr.e_ehsize<<" Bytes"<<endl;

	cout<<"=================================="<<endl;

	/* e_phoff */

	cout<<" Program Header Table Offset:     0x"<<hexformat(8)<<ehdr.e_phoff<<" ("<<std::dec<<ehdr.e_phoff<<" Bytes Offset)"<<endl;

	/* e_phentsize */
	cout<<" Program Header Size:               "<<ehdr.e_phentsize<<" Bytes"<<endl;

	/* e_phnum */
	cout<<" Number of Program Header           "<<ehdr.e_phnum<<endl;

	cout<<"=================================="<<endl;

	/* e_shoff */

	cout<<" Section Header Table Offset:     0x"<<hexformat(8)<<ehdr.e_shoff<<" ("<<std::dec<<ehdr.e_shoff<<" Bytes Offset)"<<endl;

	/* e_shentsize */
	cout<<" Section Header Size:               "<<ehdr.e_shentsize<<" Bytes"<<endl;

	/* e_shnum */
	cout<<" Number of Section Header:          "<<ehdr.e_shnum<<endl;

	/* e_shstrndx */
	cout<<" Section Header String Table Index: "<<ehdr.e_shstrndx<<endl;

	cout<<"==================================\n"<<endl;

};
