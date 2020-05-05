#include<iostream>
#include<iomanip>

#define setw_left(wd) std::setw(wd)<<std::setfill(' ')<<std::left
#define setw_right(wd) std::setw(wd)<<std::setfill(' ')<<std::right
#define hexformat(wd) std::hex<<std::setfill('0')<<std::setw(wd)
// uint8_t型は本来unsigned char なので，この関数を使ってuint8_t型の16進数を16進表示させるには，直前に(unsigned int)とかで整数型にキャストする必要がある．

#ifndef _Elf_
#define _Elf_
#include<elf.h>
#include"device.h"

class Elf{
	private:
	Elf64_Ehdr ehdr;
	public:
	Elf(void);
	Elf(Device &bd);
	~Elf(void);

	// 各構造体メンバの読み出し関数
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
	void show_ehdr(void);
};

class Section{
	private:
	Elf64_Shdr *shdr; // ポインタ表記にしておいて，コンストラクタでsizeof(Elf64_Shdr)*e_shnum だけ領域確保する．
	std::string *sh_name; // 文字通りsh_nameを格納する文字列クラス型配列．
	public:
	Section(void);
	Section(Device &bd, Elf &eh); // 動的領域確保
	~Section(void);
	void sh_parser(Device &bd, Elf &eh); // こうすればElfクラスのデータにもアクセスし放題．ただし，当然だがElfクラスのプライベートメンバにアクセスするときには，専用の読み出し関数を介さなければならない．
	void show_shdr(Device &bd, Elf &eh);
	void getsh_name(Device &bd, Elf &eh, int Dec_addr, int sec_num); // sh_nameの読み出し関数．当初は汎用性のある文字列読み出し関数を作成し，それをオーバーラップする予定だったが，よく考えたらこのクラスから他の文字列の読み出しを行うことが無い気がしたのでsh_nameの読み出し専用の関数にすることにした．
};

class Program{
	private:
	Elf64_Phdr *phdr;
	public:
	Program(void);
	Program(Device &bd, Elf &eh);
	~Program(void);
	void ph_parser(Device &bd, Elf &eh);
	void show_phdr(Device &bd, Elf &eh);
};

#endif
