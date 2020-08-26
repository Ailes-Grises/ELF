#include<iostream>
#include<string>
#include<string.h>
#include<elf.h>
#include"elf64.h"

Symbol::Symbol(std::shared_ptr<Device> bd, std::shared_ptr<Section> sh, std::string symtab_name){
	this->symtab_name=symtab_name;
	// sym_type の値によって確保する領域サイズを変更する．
	int seq=sh->Section_hash(symtab_name); // シーケンス番号取得
	this->symbol_num=sh->Sh_size(seq)/sh->Sh_entsize(seq);
	symbol = new Elf64_Sym[Symbol::symbol_num];
	st_name = new std::string[Symbol::symbol_num];
	for(int i=0;i<Symbol::symbol_num;i++) memset(&symbol[i], 0, sizeof(symbol[i]));
	this->sym_parser(bd, sh);
};

Symbol::~Symbol(void){
	delete [] symbol;
	delete [] st_name;
};

uint32_t Symbol::St_name(int seq){
	return symbol[seq].st_name;
};

unsigned char Symbol::St_info(int seq){
	return symbol[seq].st_info;
};

unsigned char Symbol::St_other(int seq){
	return symbol[seq].st_other;
};

uint16_t Symbol::St_shndx(int seq){
	return symbol[seq].st_shndx;
};

Elf64_Addr Symbol::St_value(int seq){
	return symbol[seq].st_value;
};

uint64_t Symbol::St_size(int seq){
	return symbol[seq].st_size;
};

int Symbol::Symbol_hash(std::string key){
	return symbol_hash[key];
};


void Symbol::getsym_name(std::shared_ptr<Device> bd, int addr, int seq){
	// 目的の位置(addr)までジャンプし，データを読んで，st_name[seq]に格納する．
	// int addr は該当データのバイトオフセット．

	// まずはスタックポインタを該当アドレスまでジャンプ．
	bd->setSP(addr); // ここの第二引数には，バイトオフセットをそのまま投げ込めば良い．(番地情報じゃなくて，ファイル冒頭から何バイト分の位置にあるのか)
	int i=0;
	while(1){
		char ch=bd->getChar();
		if(ch=='\0') break;
		st_name[seq]+=ch;
		i++;
	}
	return;
};

void Symbol::sym_parser(std::shared_ptr<Device> bd, std::shared_ptr<Section> sh){
	// まずは該当箇所までジャンプ．(DCを変更)
	bd->setDC(sh->Sh_offset(sh->Section_hash(Symbol::symtab_name)));

	for(int i=0;i<Symbol::symbol_num;i++){
		symbol[i].st_name=bd->get32bit();
		symbol[i].st_info=bd->get8bit();
		symbol[i].st_other=bd->get8bit();
		symbol[i].st_shndx=bd->get16bit();
		symbol[i].st_value=bd->get64bit();
		symbol[i].st_size=bd->get64bit();
	}

	// 各シンボルの名前を，文字列テーブルを格納しているセクションから読み出して初期化する．
	for(int i=0;i<Symbol::symbol_num;i++){
		// シンボル名の取得
		// シンボルテーブルの種類により，参照すべきセクション番号が変わってくる．
		if(symtab_name == ".symtab"){
			this->getsym_name(bd, sh->Sh_offset(sh->Section_hash(".strtab"))+symbol[i].st_name, i);
		}else if(symtab_name == ".dynsym"){
			this->getsym_name(bd, sh->Sh_offset(sh->Section_hash(".dynstr"))+symbol[i].st_name, i);
		}else{
			cerr<<"[Error] Invalid symbol table name. [at Symbol::sym_parser()]";
			// エラー処理
		}

		// シンボルハッシュテーブルの初期化
		symbol_hash[st_name[i]]=i;

		// for debug
		// cout<<"st_name["<<i<<"] : "<<st_name[i]<<endl;
	}
};

void Symbol::show_symtab(std::shared_ptr<Device> bd, std::shared_ptr<Section> sh){

	cout<<"Symbol table '"<<symtab_name<<"' contains "<<symbol_num<<" entries:"<<endl;

	// 文字寄せのパターン
	// r r r l l l r l
	cout<<"   Number:"; // 12文字
	cout<<setw_right(13)<<"Value"; // 16
	cout<<setw_right(15)<<"Size"; // 8
	cout<<setw_right(10)<<"Type"; // 10
	cout<<setw_right(10)<<"Bind"; // 
	cout<<setw_right(14)<<"Visibility"; // 12
	cout<<setw_right(8)<<"Index"; // 7
	cout<<setw_right(10)<<"Name"<<endl;


	for(int i=0;i<symbol_num;i++){

		/* Number */
		cout<<setw_right(9)<<std::dec<<i<<":";

		/* st_value */
		cout<<setw_right(4)<<"0x"<<hexformat(16)<<symbol[i].st_name;

		/* st_size */
		cout<<setw_right(5)<<"0x"<<hexformat(4)<<symbol[i].st_size;

		/* st_info (Type & Bind) */

		// st_info の値は，" STB_*<<4 | STT_* " という構造になっている．
		// よって，シフト演算で下位ビットから順番に読み出す．
		int STT=symbol[i].st_info & 0xf;
		int STB=symbol[i].st_info >> 4;

		// まずはType から
		switch(STT){
			case STT_NOTYPE:
				cout<<setw_right(5)<<" "<<setw_left(10)<<"NOTYPE"; break;
			case STT_OBJECT:
				cout<<setw_right(5)<<" "<<setw_left(10)<<"OBJECT"; break;
			case STT_FUNC:
				cout<<setw_right(5)<<" "<<setw_left(10)<<"FUNC"; break;
			case STT_SECTION:
				cout<<setw_right(5)<<" "<<setw_left(10)<<"SECTION"; break;
			case STT_FILE:
				cout<<setw_right(5)<<" "<<setw_left(10)<<"FILE"; break;
			case STT_COMMON:
				cout<<setw_right(5)<<" "<<setw_left(10)<<"COMMON"; break;
			case STT_TLS:
				cout<<setw_right(5)<<" "<<setw_left(10)<<"TLS"; break;
			case STT_NUM:
				cout<<setw_right(5)<<" "<<setw_left(10)<<"NUM"; break;
			// case STT_LOOS:
			// 	cout<<setw_right(5)<<" "<<setw_left(10)<<"LOOS"; break;
			case STT_GNU_IFUNC:
				cout<<setw_right(5)<<" "<<setw_left(10)<<"GNU_IFUNC"; break;
			case STT_HIOS:
				cout<<setw_right(5)<<" "<<setw_left(10)<<"HIOS"; break;
			case STT_LOPROC:
				cout<<setw_right(5)<<" "<<setw_left(10)<<"LOPROC"; break;
			case STT_HIPROC:
				cout<<setw_right(5)<<" "<<setw_left(10)<<"HIPROC"; break;
			default :
				cout<<setw_right(5)<<" "<<setw_left(10)<<"UNKNOWN"; break;
		}

		// Bind
		switch(STB){
			case STB_LOCAL:
				cout<<setw_left(9)<<"LOCAL"; break;
			case STB_GLOBAL:
				cout<<setw_left(9)<<"GLOBAL"; break;
			case STB_WEAK:
				cout<<setw_left(9)<<"WEAK"; break;
			case STB_NUM:
				cout<<setw_left(9)<<"NUM"; break;
			// case STB_LOOS:
			// 	cout<<setw_left(9)<<"LOOS"; break;
			case STB_GNU_UNIQUE:
				cout<<setw_left(9)<<"GNU_UNIQUE"; break;
			case STB_HIOS:
				cout<<setw_left(9)<<"HIOS"; break;
			case STB_LOPROC:
				cout<<setw_left(9)<<"LOPROC"; break;
			case STB_HIPROC:
				cout<<setw_left(9)<<"HIPROC"; break;
			default :
				cout<<setw_left(9)<<"NOTYPE"; break;
		}

		/* st_other (Visibility) */
		switch(symbol[i].st_other){
			case STV_DEFAULT:
				cout<<setw_left(12)<<"DEFAULT"; break;
			case STV_INTERNAL:
				cout<<setw_left(12)<<"INTERNAL"; break;
			case STV_HIDDEN:
				cout<<setw_left(12)<<"HIDDEN"; break;
			case STV_PROTECTED:
				cout<<setw_left(12)<<"PROTECTED"; break;
			default :
				cout<<setw_left(12)<<"UNKNOWN"; break;
		}


		/* st_shndx (Index) */
		cout<<std::dec<<setw_right(4)<<symbol[i].st_shndx;

		/* st_name */
		cout<<setw_right(3)<<" "<<st_name[i]<<endl;
	}
	cout<<endl;

};

