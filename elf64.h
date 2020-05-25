#include<iostream>
#include<iomanip>
#include<unordered_map>

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
	uint16_t E_shstrndx(void); // eh.e_shstrndx 番目のセクションにセクションヘッダ名の文字列が入っている．

	void eh_parser(Device &bd);
	void show_ehdr(void);
};

class Section{
	private:
	Elf64_Shdr *shdr; // ポインタ表記にしておいて，コンストラクタでsizeof(Elf64_Shdr)*e_shnum だけ領域確保する．
	std::string *sh_name; // 文字通りsh_nameを格納する文字列クラス型配列．
	std::unordered_map<std::string, int> section_hash; // セクション名と番号のハッシュテーブル．

	public:
	Section(Device &bd, Elf &eh); // 動的領域確保
	~Section(void);
	uint32_t Sh_name(int seq);
	uint32_t Sh_type(int seq);
	uint64_t Sh_flags(int seq);
	Elf64_Addr Sh_addr(int seq);
	Elf64_Off Sh_offset(int seq);
	uint64_t Sh_size(int seq);
	uint32_t Sh_link(int seq);
	uint32_t Sh_info(int seq);
	uint64_t Sh_addralign(int seq);
	uint64_t Sh_entsize(int seq);

	void sh_parser(Device &bd, Elf &eh); // こうすればElfクラスのデータにもアクセスし放題．ただし，当然だがElfクラスのプライベートメンバにアクセスするときには，専用の読み出し関数を介さなければならない．
	void getsh_name(Device &bd, Elf &eh, int addr, int sec_num); // sh_nameの読み出し関数．当初は汎用性のある文字列読み出し関数を作成し，それをオーバーラップする予定だったが，よく考えたらこのクラスから他の文字列の読み出しを行うことが無い気がしたのでsh_nameの読み出し専用の関数にすることにした．
	int Section_hash(std::string key); // ハッシュ関数．
	void show_shdr(Device &bd, Elf &eh);
};

class Program{
	private:
	Elf64_Phdr *phdr;
	public:
	Program(Device &bd, Elf &eh);
	~Program(void);
	void ph_parser(Device &bd, Elf &eh);
	void show_phdr(Device &bd, Elf &eh);
};

class Symbol{
	private:
	Elf64_Sym *symbol; // シンボルテーブルを格納する配列．
	std::string *st_name; // st_name を格納する配列．
	std::unordered_map<std::string, int> symbol_hash; // シンボル名とシーケンス番号のハッシュ．要するに名前から該当する配列のシーケンス番号が分かるようにしたい．
	std::string symtab_name;
	unsigned int symbol_num;

	public:
	Symbol(Device &bd, Section &sh, std::string symtab_name);
	~Symbol(void);
	uint32_t St_name(int seq);
	unsigned char St_info(int seq);
	unsigned char St_other(int seq);
	uint16_t St_shndx(int seq);
	Elf64_Addr St_value(int seq);
	uint64_t St_size(int seq);
	int Symbol_hash(std::string key); // ハッシュ関数．

	void sym_parser(Device &bd, Section &sh);
	void getsym_name(Device &bd, int addr, int sec_num);
	void show_symtab(Device &bd, Section &sh);
};

#endif
