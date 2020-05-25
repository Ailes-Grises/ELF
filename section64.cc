#include<iostream>
#include<iomanip>
#include<string>
#include<string.h>
#include<elf.h>
#include"elf64.h"

Section::Section(Device &bd, Elf &eh){
	shdr=new Elf64_Shdr[eh.E_shnum()];
	sh_name=new std::string[eh.E_shnum()]; // sh_name格納用のデータ領域を確保．
	for(int i=0;i<eh.E_shnum();i++) memset(&shdr[i], 0, sizeof(shdr[i]));
	this->sh_parser(bd, eh); // ここでパーサを呼び出す
};

Section::~Section(void){
	delete [] shdr;
	delete [] sh_name;
};

uint32_t Section::Sh_name(int seq){
	return shdr[seq].sh_name;
};

uint32_t Section::Sh_type(int seq){
	return shdr[seq].sh_type;
};

uint64_t Section::Sh_flags(int seq){
	return shdr[seq].sh_flags;
};

Elf64_Addr Section::Sh_addr(int seq){
	return shdr[seq].sh_addr;
};

Elf64_Off Section::Sh_offset(int seq){
	return shdr[seq].sh_offset;
};

uint64_t Section::Sh_size(int seq){
	return shdr[seq].sh_size;
};

uint32_t Section::Sh_link(int seq){
	return shdr[seq].sh_link;
};

uint32_t Section::Sh_info(int seq){
	return shdr[seq].sh_info;
};

uint64_t Section::Sh_addralign(int seq){
	return shdr[seq].sh_addralign;
};

uint64_t Section::Sh_entsize(int seq){
	return shdr[seq].sh_entsize;
};


void Section::sh_parser(Device &bd, Elf &eh){ // ここでElfクラスの情報が必要なので，クラスの参照をコンストラクタの引数として受け取っている．

	// Device クラスのデータカウンタdcの位置をeh.e_shoff に移動する．
	bd.setDC(eh.E_shoff()); 
	// eh.showDC(); // 動作確認済み．

	for(int i=0;i<eh.E_shnum();i++){
		// 仕様書に従って構造体へダンプ
		shdr[i].sh_name=bd.get32bit();
		shdr[i].sh_type=bd.get32bit();
		shdr[i].sh_flags=bd.get64bit();
		shdr[i].sh_addr=bd.get64bit();
		shdr[i].sh_offset=bd.get64bit();
		shdr[i].sh_size=bd.get64bit();
		shdr[i].sh_link=bd.get32bit();
		shdr[i].sh_info=bd.get32bit();
		shdr[i].sh_addralign=bd.get64bit();
		shdr[i].sh_entsize=bd.get64bit();
	}

		// セクション名取得
		// shdr[eh.E_shstrndx()]の情報を使っているので，上のループと同時に実行してはならない
		// 仕組みとしては，.shstrtab セクションのバイトオフセットから，各セクションの名前が格納されている場所のバイトオフセットであるshdr[i].sh_nameの位置を指定している．
	for(int i=0;i<eh.E_shnum();i++){
		this->getsh_name(bd, eh, shdr[eh.E_shstrndx()].sh_offset+shdr[i].sh_name, i);
		section_hash[sh_name[i]]=i;
	}

	// for debug
	// cout<<".dynsym : "<<section_hash[".dynsym"]<<endl;
	// cout<<".symtab : "<<section_hash[".symtab"]<<endl;
};

void Section::getsh_name(Device &bd, Elf &eh, int addr, int sec_num){
	// 目的の位置(addr)までジャンプし，データを読んで(getDATA)，sh_name[sec_num]に格納する．
	// int addr は該当データのバイトオフセット．

	// まずはスタックポインタを該当アドレスまでジャンプ．
	bd.setSP(addr); // ここの第二引数には，バイトオフセットをそのまま投げ込めば良い．(番地情報じゃなくて，ファイル冒頭から何バイト分の位置にあるのか)
	int i=0;
	while(1){
		char ch=bd.getChar();
		if(ch=='\0') break;
		sh_name[sec_num]+=ch;
		i++;
	}

	return;
};

int Section::Section_hash(std::string key){
	return section_hash[key];
}

void Section::show_shdr(Device &bd, Elf &eh){

	cout<<"There are "<<eh.E_shnum()<<" section headers, starting at offset "<<"0x"<<hexformat(4)<<eh.E_shoff()<<":\n"<<endl;

	cout<<"Section Header: "<<endl;
	cout<<"  [number]  ";
	cout<<setw_left(18)<<"Name";
	cout<<setw_left(18)<<"Type";
	cout<<setw_left(18)<<"Address";
	cout<<setw_left(15)<<"Offset"<<endl;
	cout<<"            "<<setw_left(18)<<"Size";
	cout<<setw_left(18)<<"EntSize";
	cout<<setw_left(18)<<"Flag  Link  Info  Alignment\n"<<endl;

	for(int i=0;i<eh.E_shnum();i++){

		/* Name */
		cout<<"   [ "<<std::dec<<setw_right(2)<<i<<" ]   "<<setw_left(19)<<sh_name[i]; // std::setwの使用なのか，何故か末尾に謎の文字が入ってしまうため，これを考慮して19としている．

		/* Type */
		switch(shdr[i].sh_type){
			case SHT_PROGBITS:
				cout<<setw_left(18)<<"PROGBITS"; break;
			case SHT_SYMTAB:
				cout<<setw_left(18)<<"SYMTAB"; break;
			case SHT_STRTAB:
				cout<<setw_left(18)<<"STRTAB"; break;
			case SHT_RELA:
				cout<<setw_left(18)<<"RELA"; break;
			case SHT_HASH:
				cout<<setw_left(18)<<"HASH"; break;
			case SHT_DYNAMIC:
				cout<<setw_left(18)<<"DYNAMIC"; break;
			case SHT_NOTE:
				cout<<setw_left(18)<<"NOTE"; break;
			case SHT_NOBITS:
				cout<<setw_left(18)<<"NOBITS"; break;
			case SHT_REL:
				cout<<setw_left(18)<<"REL"; break;
			case SHT_SHLIB:
				cout<<setw_left(18)<<"SHLIB"; break;
			case SHT_DYNSYM:
				cout<<setw_left(18)<<"DYNSYM"; break;
			case SHT_LOPROC:
				cout<<setw_left(18)<<"LOPROC"; break;
			case SHT_HIPROC:
				cout<<setw_left(18)<<"HIPROC"; break;
			case SHT_LOUSER:
				cout<<setw_left(18)<<"LOUSER"; break;
			case SHT_HIUSER:
				cout<<setw_left(18)<<"HIUSER"; break;
			default:
				cout<<setw_left(18)<<"NULL"; break;
		}

		/* Address */
		cout<<"0x"<<std::right<<hexformat(14)<<shdr[i].sh_addr<<"  ";

		/* Offset */
		cout<<"0x"<<std::right<<hexformat(8)<<shdr[i].sh_offset<<endl;

		/* Size */
		cout<<"            "<<hexformat(16)<<shdr[i].sh_size<<"  ";

		/* Entsize */
		cout<<"0x"<<hexformat(14)<<shdr[i].sh_entsize<<"    ";

		/* Flags */
		// sh_flagsの値は，/usr/include/elf.h で定義されているSHF_* の値(この値は2進数)の総和(合計11bitある)を16進数で表したものである．
		// よって，sh_flagsの値を2進数に直し，1のフラグが立っている部分が権限である．
		// これを調べるには，SHF_* とsh_flagsの論理積(&)を取れば良い．
		// /usr/include/elf.h にマクロで記述されている "(1 << 2)"などは，(0100)等のシフト演算表記なので，これ自体が数字である(命令ではない)．

		std::string flags;

		if((shdr[i].sh_flags & SHF_WRITE)==SHF_WRITE) flags+='W';
		if((shdr[i].sh_flags & SHF_ALLOC)==SHF_ALLOC) flags+='A';
		if((shdr[i].sh_flags & SHF_EXECINSTR)==SHF_EXECINSTR) flags+='X';
		if((shdr[i].sh_flags & SHF_MERGE)==SHF_MERGE) flags+='M';
		if((shdr[i].sh_flags & SHF_STRINGS)==SHF_STRINGS) flags+='S';
		if((shdr[i].sh_flags & SHF_INFO_LINK)==SHF_INFO_LINK) flags+='I';
		if((shdr[i].sh_flags & SHF_LINK_ORDER)==SHF_LINK_ORDER) flags+='L';
		if((shdr[i].sh_flags & SHF_OS_NONCONFORMING)==SHF_OS_NONCONFORMING) flags+='O';
		if((shdr[i].sh_flags & SHF_GROUP)==SHF_GROUP) flags+='G';
		if((shdr[i].sh_flags & SHF_TLS)==SHF_TLS) flags+='T';
		if((shdr[i].sh_flags & SHF_COMPRESSED)==SHF_COMPRESSED) flags+='C';
		if((shdr[i].sh_flags & SHF_MASKOS)==SHF_MASKOS) flags+='o';
		if((shdr[i].sh_flags & SHF_EXCLUDE)==SHF_EXCLUDE) flags+='E';
		if((shdr[i].sh_flags & SHF_MASKPROC)==SHF_MASKPROC) flags+='p';

		cout<<setw_left(4)<<flags;

		/* Link */
		cout<<setw_right(3)<<shdr[i].sh_link;

		/* Info */
		cout<<setw_right(6)<<shdr[i].sh_info;

		/* Alignment */
		cout<<"       "<<shdr[i].sh_addralign<<"\n"<<endl;
	}

	cout<<"Key to Flags:"<<endl;
	cout<<"  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),"<<endl;
	cout<<"  L (link order), O (extra OS processing required), G (group), T (TLS),"<<endl;
	cout<<"  C (compressed), x (unknown), o (OS specific), E (exclude),"<<endl;
	cout<<"  l (large), p (processor specific)\n\n"<<endl;

	return;
};

