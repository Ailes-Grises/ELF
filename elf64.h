#include<iostream>
#include<iomanip>
#include<memory>
#include<unordered_map>

#define setw_left(wd) std::setw(wd) << std::setfill(' ') << std::left
#define setw_right(wd) std::setw(wd) << std::setfill(' ') << std::right
#define hexformat(wd) std::hex << std::setfill('0') << std::setw(wd)
// uint8_t型は本来unsigned char なので，この関数を使ってuint8_t型の16進数を16進表示させるには，直前に(unsigned int)とかで整数型にキャストする必要がある．

#ifndef _Elf_
#define _Elf_
#include<elf.h>
#include"device.h"

// ELF ヘッダの解析
class Elf{
	private:
	Elf64_Ehdr ehdr;
	public:
	Elf(std::shared_ptr<Device> bd);
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

	void eh_parser(std::shared_ptr<Device> bd);
	void show_ehdr(void);
};

// Section ヘッダの解析
class Section{
	private:
	Elf64_Shdr *shdr; // ポインタ表記にしておいて，コンストラクタでsizeof(Elf64_Shdr)*e_shnum だけ領域確保する．
	std::string *sh_name; // sh_nameの文字列を格納する文字列クラス型配列．
	std::unordered_map<std::string, int> section_hash; // セクション名と番号のハッシュテーブル．

	public:
	Section(std::shared_ptr<Device> bd, std::shared_ptr<Elf> eh); // 動的領域確保
	~Section(void);

	// 各構造体メンバの読み出し関数
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

	void sh_parser(std::shared_ptr<Device> bd, std::shared_ptr<Elf> eh); // こうすればElfクラスのデータにもアクセスし放題．ただし，当然だがElfクラスのプライベートメンバにアクセスするときには，専用の読み出し関数を介さなければならない．
	void getsh_name(std::shared_ptr<Device> bd, std::shared_ptr<Elf> eh, int addr, int sec_num); // 各セクションの名前を読み出してstd::string sh_name を初期化する関数．
	int Section_hash(std::string key); // ハッシュ関数．
	void show_shdr(std::shared_ptr<Device> bd, std::shared_ptr<Elf> eh);
};

// Program ヘッダの解析
class Program{
	private:
	Elf64_Phdr *phdr;
	public:
	Program(std::shared_ptr<Device> bd, std::shared_ptr<Elf> eh);
	~Program(void);
	void ph_parser(std::shared_ptr<Device> bd, std::shared_ptr<Elf> eh);
	void show_phdr(std::shared_ptr<Device> bd, std::shared_ptr<Elf> eh);
};

// ターゲットとなるある一つのセクションに格納されているシンボルに関する情報を解析するクラス
class Symbol{
	private:
	Elf64_Sym *symbol; // シンボルテーブル(構造体)を格納する配列．
	std::string *st_name; // st_name の文字列を格納する配列．
	std::unordered_map<std::string, int> symbol_hash; // シンボル名とシーケンス番号のハッシュ．要するに名前から該当する配列のシーケンス番号が分かるようにしたい．
	std::string symtab_name; // 解析対象セクションの名前．(.dynsym等)
	unsigned int symbol_num; // 解析対象セクションに格納されているシンボルテーブル(構造体)の個数．

	public:
	Symbol(std::shared_ptr<Device> bd, std::shared_ptr<Section> sh, std::string symtab_name);
	~Symbol(void);

	// 各構造体メンバの読み出し関数
	uint32_t St_name(int seq);
	unsigned char St_info(int seq);
	unsigned char St_other(int seq);
	uint16_t St_shndx(int seq);
	Elf64_Addr St_value(int seq);
	uint64_t St_size(int seq);

	int Symbol_hash(std::string key); // ハッシュ関数．

	void sym_parser(std::shared_ptr<Device> bd, std::shared_ptr<Section> sh);
	void getsym_name(std::shared_ptr<Device> bd, int addr, int sec_num); // シンボルの名前を読み出してstd::string st_name を初期化する関数．
	void show_symtab(std::shared_ptr<Device> bd, std::shared_ptr<Section> sh);
};

// ダイナミックリンクの解析
class Dynamic{
	private:
	Elf64_Dyn *dynamic; // 構造体型の配列 (自作の_DYNAMIC[] の実装)
	unsigned int dynamic_num; // 構造体の個数
	unsigned int dynamic_seq; // .dynamic セクションのシーケンス番号
	public:
	Dynamic(void);
	Dynamic(std::shared_ptr<Device> bd, std::shared_ptr<Section> sh);
	~Dynamic(void);

	// 各構造体メンバの読み出し関数
	int64_t D_tag(unsigned int seq);
	uint64_t D_val(unsigned int seq); // union メンバなので気をつけて運用すること．
	uint64_t D_ptr(unsigned int seq);

	void dyn_parser(std::shared_ptr<Device> bd, std::shared_ptr<Section> sh);
	void show_dynamic(std::shared_ptr<Device> bd, std::shared_ptr<Section> sh);
};

#endif
