#include<iostream>
#include<iomanip>
#include<string>
#include<elf.h>
#include"elf64.h"

Section::Section(void){
};

Section::Section(Elf &eh){
	shdr=new Elf64_Shdr[eh.E_shnum()];
	sh_name=new std::string[eh.E_shnum()]; // sh_name格納用のデータ領域を確保．
};

Section::~Section(void){
};

void Section::sh_parser(Device &bd, Elf &eh, Section &sh){ // ここでElfクラスの情報が必要なので，クラスの参照をコンストラクタの引数として受け取っている．

	// Device クラスのデータカウンタdcの位置をeh.e_shoff に移動する．
	bd.setDC(bd, eh.E_shoff()); 
	// eh.showDC(eh); // 動作確認済み．

	for(int i=0;i<eh.E_shnum();i++){
		// 仕様書に従って構造体へダンプ
		sh.shdr[i].sh_name=bd.get32bit(bd);
		sh.shdr[i].sh_type=bd.get32bit(bd);
		sh.shdr[i].sh_flags=bd.get64bit(bd);
		sh.shdr[i].sh_addr=bd.get64bit(bd);
		sh.shdr[i].sh_offset=bd.get64bit(bd);
		sh.shdr[i].sh_size=bd.get64bit(bd);
		sh.shdr[i].sh_link=bd.get32bit(bd);
		sh.shdr[i].sh_info=bd.get32bit(bd);
		sh.shdr[i].sh_addralign=bd.get64bit(bd);
		sh.shdr[i].sh_entsize=bd.get64bit(bd);
	}
};

void Section::getsh_name(Device &bd, Elf &eh, Section &sh, int Dec_addr, int sec_num){
	// 目的の位置(Dec_addr)までジャンプし，データを読んで(getDATA)，sh_name[sec_num]に格納する．
	// int Dec_addr は該当データのバイトオフセット．

	// まずはスタックポインタを該当アドレスまでジャンプ．
	bd.setSP(bd, Dec_addr); // ここの第二引数には，バイトオフセットをそのまま投げ込めば良い．(番地情報じゃなくて，ファイル冒頭から何バイト分の位置にあるのか)
	int i=0;
	while(1){
		sh_name[sec_num]+=bd.getChar(bd);
		if(sh_name[sec_num][i]=='\0') break; // このままだと文字列の最後に'\0'が2個格納される．まぁ問題は無いかもだけど．
		i++;
	}

	return;
};

void Section::show_shdr(Device &bd, Elf &eh, Section &sh){

	std::cout<<"There are "<<eh.E_shnum()<<" section headers, starting at offset "<<"0x"<<hexformat(4)<<eh.E_shoff()<<":\n"<<std::endl;

	std::cout<<"Section Header: "<<std::endl;
	std::cout<<"  [number]  "<<setw_left(18)<<"Name"<<setw_left(18)<<"Type"<<setw_left(18)<<"Address"<<setw_left(15)<<"Offset"<<std::endl;
	std::cout<<"            "<<setw_left(18)<<"Size"<<setw_left(18)<<"EntSize"<<setw_left(18)<<"Flag  Link  Info  Alignment\n"<<std::endl;

	for(int i=0;i<eh.E_shnum();i++){
		// セクション名取得
		sh.getsh_name(bd, eh, sh, shdr[eh.E_shstrndx()].sh_offset+shdr[i].sh_name, i);
		// 仕組みとしては，.shstrtab セクションのバイトオフセットから，各セクションの名前が格納されている場所のバイトオフセットであるshdr[i].sh_nameの位置を指定している．

		/* Name */
		std::cout<<"   [ "<<std::dec<<setw_right(2)<<i<<" ]   "<<setw_left(19)<<sh_name[i]; // std::setwの使用なのか，何故か末尾に謎の文字が入ってしまうため，これを考慮して19としている．

		/* Type */
		switch(sh.shdr[i].sh_type){
			case SHT_PROGBITS:
				std::cout<<setw_left(18)<<"PROGBITS"; break;
			case SHT_SYMTAB:
				std::cout<<setw_left(18)<<"SYMTAB"; break;
			case SHT_STRTAB:
				std::cout<<setw_left(18)<<"STRTAB"; break;
			case SHT_RELA:
				std::cout<<setw_left(18)<<"RELA"; break;
			case SHT_HASH:
				std::cout<<setw_left(18)<<"HASH"; break;
			case SHT_DYNAMIC:
				std::cout<<setw_left(18)<<"DYNAMIC"; break;
			case SHT_NOTE:
				std::cout<<setw_left(18)<<"NOTE"; break;
			case SHT_NOBITS:
				std::cout<<setw_left(18)<<"NOBITS"; break;
			case SHT_REL:
				std::cout<<setw_left(18)<<"REL"; break;
			case SHT_SHLIB:
				std::cout<<setw_left(18)<<"SHLIB"; break;
			case SHT_DYNSYM:
				std::cout<<setw_left(18)<<"DYNSYM"; break;
			case SHT_LOPROC:
				std::cout<<setw_left(18)<<"LOPROC"; break;
			case SHT_HIPROC:
				std::cout<<setw_left(18)<<"HIPROC"; break;
			case SHT_LOUSER:
				std::cout<<setw_left(18)<<"LOUSER"; break;
			case SHT_HIUSER:
				std::cout<<setw_left(18)<<"HIUSER"; break;
			default:
				std::cout<<setw_left(18)<<"NULL"; break;
		}

		/* Address */
		std::cout<<"0x"<<std::right<<hexformat(14)<<sh.shdr[i].sh_addr<<"  ";

		/* Offset */
		std::cout<<"0x"<<std::right<<hexformat(8)<<sh.shdr[i].sh_offset<<std::endl;

		/* Size */
		std::cout<<"            "<<hexformat(16)<<sh.shdr[i].sh_size<<"  ";

		/* Entsize */
		std::cout<<"0x"<<hexformat(14)<<shdr[i].sh_entsize<<"    ";

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

		std::cout<<setw_left(4)<<flags;

		/* Link */
		std::cout<<setw_right(3)<<shdr[i].sh_link;

		/* Info */
		std::cout<<setw_right(6)<<shdr[i].sh_info;

		/* Alignment */
		std::cout<<"       "<<shdr[i].sh_addralign<<"\n"<<std::endl;
	}

	std::cout<<"Key to Flags:"<<std::endl;
	std::cout<<"  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),"<<std::endl;
	std::cout<<"  L (link order), O (extra OS processing required), G (group), T (TLS),"<<std::endl;
	std::cout<<"  C (compressed), x (unknown), o (OS specific), E (exclude),"<<std::endl;
	std::cout<<"  l (large), p (processor specific)\n\n"<<std::endl;

	return;
};

