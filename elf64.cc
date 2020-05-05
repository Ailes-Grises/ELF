#include<iostream>
#include<iomanip>
#include<string.h>
#include<elf.h>
#include"elf64.h"

Elf::Elf(void){
	memset(&ehdr, 0, sizeof(ehdr));
};

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

void Elf::eh_parser(Device &obj){
	for(int i=0;i<EI_NIDENT;i++) ehdr.e_ident[i]=obj.get8bit(obj);

	ehdr.e_type=obj.get16bit(obj);
	ehdr.e_machine=obj.get16bit(obj);
	ehdr.e_version=obj.get32bit(obj);
	ehdr.e_entry=obj.get64bit(obj);
	ehdr.e_phoff=obj.get64bit(obj);
	ehdr.e_shoff=obj.get64bit(obj);
	ehdr.e_flags=obj.get32bit(obj);
	ehdr.e_ehsize=obj.get16bit(obj);
	ehdr.e_phentsize=obj.get16bit(obj);
	ehdr.e_phnum=obj.get16bit(obj);
	ehdr.e_shentsize=obj.get16bit(obj);
	ehdr.e_shnum=obj.get16bit(obj);
	ehdr.e_shstrndx=obj.get16bit(obj);
};

void Elf::show_ehdr(void){
	std::cout<<"ELF Header:"<<std::endl;

	/* e_ident[EI_NIDENT] */

	std::cout<<" Magic:    ";
	for(int i=0;i<EI_NIDENT;i++) std::cout<<hexformat(2)<<(unsigned int)ehdr.e_ident[i]<<" ";
	std::cout<<std::endl;

	switch(ehdr.e_ident[EI_CLASS]){
		case ELFCLASS64:
			std::cout<<" Class:                             ELF64"<<std::endl; break;
		case ELFCLASS32:
			std::cout<<" Class:                             ELF32"<<std::endl; break;
		default:
			std::cout<<" Class:                             Invalid Class"<<std::endl; break;
	}

	switch(ehdr.e_ident[EI_DATA]){
		case ELFDATA2LSB:
			std::cout<<" Data:                              2's Complement, Little Endian"<<std::endl; break;
		case ELFDATA2MSB:
			std::cout<<" Data:                              2's Complement, Big Endian"<<std::endl; break;
		default:
			std::cout<<" Data:                              Invalid Data Format"<<std::endl; break;
	}

	switch(ehdr.e_ident[EI_VERSION]){
		case EV_CURRENT:
			std::cout<<" Version:                           1 (current)"<<std::endl; break;
		default:
			std::cout<<" Version:                           0 (illegal)"<<std::endl; break;
	}

	switch(ehdr.e_ident[EI_OSABI]){
		case ELFOSABI_SYSV:
			std::cout<<" OS/ABI:                            UNIX - System V"<<std::endl; break;
		case ELFOSABI_HPUX:
			std::cout<<" OS/ABI:                            HP-UX"<<std::endl; break;
		case ELFOSABI_NETBSD:
			std::cout<<" OS/ABI:                            NetBSD"<<std::endl; break;
		case ELFOSABI_LINUX:
			std::cout<<" OS/ABI:                            Linux"<<std::endl; break;
		case ELFOSABI_SOLARIS:
			std::cout<<" OS/ABI:                            Solaris"<<std::endl; break;
		case ELFOSABI_IRIX:
			std::cout<<" OS/ABI:                            IRIX"<<std::endl; break;
		case ELFOSABI_FREEBSD:
			std::cout<<" OS/ABI:                            FreeBSD"<<std::endl; break;
		case ELFOSABI_TRU64:
			std::cout<<" OS/ABI:                            TRU64 - UNIX"<<std::endl; break;
		case ELFOSABI_ARM:
			std::cout<<" OS/ABI:                            ARM"<<std::endl; break;
		case ELFOSABI_STANDALONE:
			std::cout<<" OS/ABI:                            Standalone"<<std::endl; break;
		default:
			std::cout<<" OS/ABI:                            UNIX - System V"<<std::endl; break;
	}

	// ここだけ決め打ち
	std::cout<<" ABI Version:                       0"<<std::endl;

	/* e_type */

	switch(ehdr.e_type){
		case ET_REL: 
			std::cout<<" Type:                              REL (Relocatable File)"<<std::endl; break;
		case ET_EXEC: 
			std::cout<<" Type:                              EXEC (Executable File)"<<std::endl; break;
		case ET_DYN: 
			std::cout<<" Type:                              DYN (Shared Object File)"<<std::endl; break;
		case ET_CORE: 
			std::cout<<" Type:                              CORE (Core File)"<<std::endl; break;
		default: 
			std::cout<<" Type:                              NONE (Invalid Type)"<<std::endl; break;
	}

	/* e_machine */

	switch(ehdr.e_machine){
		case EM_M32: 
			std::cout<<" Architecture:                      AT&T WE 32100"<<std::endl; break;
		case EM_SPARC: 
			std::cout<<" Architecture:                      Sun Microsystems SPARC"<<std::endl; break;
		case EM_386: 
			std::cout<<" Architecture:                      Intel 80386"<<std::endl; break;
		case EM_68K: 
			std::cout<<" Architecture:                      Motorola 68000"<<std::endl; break;
		case EM_88K: 
			std::cout<<" Architecture:                      Motorola 88000"<<std::endl; break;
		case EM_860: 
			std::cout<<" Architecture:                      Intel 80860"<<std::endl; break;
		case EM_MIPS: 
			std::cout<<" Architecture:                      MIPS RS3000"<<std::endl; break;
		case EM_PARISC: 
			std::cout<<" Architecture:                      HP/PA"<<std::endl; break;
		case EM_SPARC32PLUS: 
			std::cout<<" Architecture:                      SPARC with Extended Instruction Set"<<std::endl; break;
		case EM_PPC: 
			std::cout<<" Architecture:                      PowerPC"<<std::endl; break;
		case EM_PPC64: 
			std::cout<<" Architecture:                      PowerPC 64-bit"<<std::endl; break;
		case EM_S390: 
			std::cout<<" Architecture:                      IBM S/390"<<std::endl; break;
		case EM_ARM: 
			std::cout<<" Architecture:                      ARM"<<std::endl; break;
		case EM_SH: 
			std::cout<<" Architecture:                      Renesas SuperH"<<std::endl; break;
		case EM_SPARCV9: 
			std::cout<<" Architecture:                      SPARC v9 64-bit"<<std::endl; break;
		case EM_IA_64: 
			std::cout<<" Architecture:                      Intel Itanium"<<std::endl; break;
		case EM_X86_64: 
			std::cout<<" Architecture:                      AMD x86-64"<<std::endl; break;
		case EM_VAX: 
			std::cout<<" Architecture:                      DEC Vax"<<std::endl; break;
		default: 
			std::cout<<" Architecture:                      Unknown Architecture"<<std::endl; break;
	}

	/* e_version */

	switch(ehdr.e_version){
		case EV_CURRENT:
			std::cout<<" Version:                           Current Version(0x01)"<<std::endl; break;
		default:
			std::cout<<" Version:                           Unknown Version(0x00)"<<std::endl; break;
	}

	/* e_entry */

	std::cout<<" Entry Point Address:               0x"<<hexformat(8)<<ehdr.e_entry<<std::endl;

	/* e_flags */

	std::cout<<" Flag:                              0x"<<hexformat(2)<<ehdr.e_flags<<std::endl;

	/* e_ehsize */

	std::cout<<" Elf Header Size:                   "<<std::dec<<ehdr.e_ehsize<<" Bytes"<<std::endl;

	std::cout<<"=================================="<<std::endl;

	/* e_phoff */

	std::cout<<" Program Header Table Offset:     0x"<<hexformat(8)<<ehdr.e_phoff<<" ("<<std::dec<<ehdr.e_phoff<<" Bytes Offset)"<<std::endl;

	/* e_phentsize */
	std::cout<<" Program Header Size:               "<<ehdr.e_phentsize<<" Bytes"<<std::endl;

	/* e_phnum */
	std::cout<<" Number of Program Header           "<<ehdr.e_phnum<<std::endl;

	std::cout<<"=================================="<<std::endl;

	/* e_shoff */

	std::cout<<" Section Header Table Offset:     0x"<<hexformat(8)<<ehdr.e_shoff<<" ("<<std::dec<<ehdr.e_shoff<<" Bytes Offset)"<<std::endl;

	/* e_shentsize */
	std::cout<<" Section Header Size:               "<<ehdr.e_shentsize<<" Bytes"<<std::endl;

	/* e_shnum */
	std::cout<<" Number of Section Header:          "<<ehdr.e_shnum<<std::endl;

	/* e_shstrndx */
	std::cout<<" Section Header String Table Index: "<<ehdr.e_shstrndx<<std::endl;

	std::cout<<"==================================\n"<<std::endl;

};
