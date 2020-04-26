#include<iostream>
#include<iomanip>
#include<string.h>
#include<elf.h>
#include"elf.h"
#include<stdlib.h>

Section::Section(void){
}

Section::Section(Elf &eh){
	shdr=(Elf64_Shdr*)malloc(eh.E_shnum()*sizeof(Elf64_Shdr)); // ここでElfクラスの情報が必要なので，クラスの参照をコンストラクタの引数として受け取っている．
}

Section::~Section(void){
}

void Section::sh_parser(Elf &eh, Section &sh){ // ここでElfクラスの情報が必要なので，クラスの参照をコンストラクタの引数として受け取っている．

	// Device クラスのデータカウンタdcの位置をeh.e_shoff に移動する．
	eh.SetDC(eh, eh.E_shoff()); 
	// eh.ShowDC(eh); // 動作確認済み．

	for(int i=0;i<eh.E_shnum();i++){
		// 仕様書に従って構造体へダンプ
		sh.shdr[i].sh_name=eh.get32bit(eh);
		sh.shdr[i].sh_type=eh.get32bit(eh);
		sh.shdr[i].sh_flags=eh.get64bit(eh);
		sh.shdr[i].sh_addr=eh.get64bit(eh);
		sh.shdr[i].sh_offset=eh.get64bit(eh);
		sh.shdr[i].sh_size=eh.get64bit(eh);
		sh.shdr[i].sh_link=eh.get32bit(eh);
		sh.shdr[i].sh_info=eh.get32bit(eh);
		sh.shdr[i].sh_addralign=eh.get64bit(eh);
		sh.shdr[i].sh_entsize=eh.get64bit(eh);
	}
}

void Section::showshdr(Elf &eh, Section &sh){
	// fprintf(stderr, "sh.shdr[34].sh_size : %x\n", sh.shdr[34].sh_size);

	std::cout<<"Section Header: "<<std::endl;
	std::cout<<"  [number]  Name               Type               Address           Offset"<<std::endl;
	std::cout<<"            Size               EntSize            Flag  Link  Info  Alignment"<<std::endl;

	for(int i=0;i<eh.E_shnum();i++){
		// std::cout<<"   [ "<<std::setw(2)<<i<<" ]  "<<std::setw(16)<<"  Type               Address           Offset"<<std::endl;
		// std::cout<<"            Size               EntSize            Flag  Link  Info  Alignment"<<std::endl;
	}
	return;
}

