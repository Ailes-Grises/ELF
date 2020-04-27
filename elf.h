#include<iostream>
#include<iomanip>

#define hexformat(wd) std::hex<<std::setfill('0')<<std::setw(wd)
// uint8_t型は本来unsigned char なので，この関数を使ってuint8_t型の16進数を16進表示させるには，直前に(unsigned int)とかで整数型にキャストする必要がある．

#ifndef _Elf_
#define _Elf_
#include<elf.h>
#include"device.h"

class Elf{
	private:
	//int bit; // CPUのビット数．めんどくさい．
	Elf64_Ehdr ehdr;
	public:
	Elf(void);
	~Elf(void);

	// 各構造体メンバの読み出し関数(ただし64bit版しか実装していない．)
	unsigned char E_ident(int Nident);
	uint16_t E_type(void);
	uint16_t E_machine(void);
	uint32_t E_version(void);
	Elf64_Addr E_entry(void);
	Elf64_Off E_phoff(void);
	Elf64_Off E_shoff(void);
	uint32_t E_flags(void);
	uint16_t E_ehsize(void);
	uint16_t E_phentsize(void);
	uint16_t E_phnum(void);
	uint16_t E_shentsize(void);
	uint16_t E_shnum(void);
	uint16_t E_shstrndx(void);

	void eh_parser(Device &obj);
	void showehdr(void);
};

class Section{
	private:
	Elf64_Shdr *shdr; // ポインタ表記にしておいて，コンストラクタでsizeof(Elf64_Shdr)*e_shnum だけ領域確保する．
	public:
	Section(void);
	Section(Elf &eh); // 動的領域確保
	~Section(void);
	void sh_parser(Device &bd, Elf &eh, Section &sh); // こうすればElfクラスのデータにもアクセスし放題．ただし，当然だがElfクラスのプライベートメンバにアクセスするときには，専用の読み出し関数を介さなければならない．
	void showshdr(Elf &eh, Section &sh);
	// もし他のクラスからこのクラスの構造体型配列shdrの各メンバにアクセスしたいのならば，Elfクラスのように，各メンバへのアクセス関数を作成する必要がある．
};

#endif
