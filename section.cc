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

void Section::sh_parser(Device &bd, Elf &eh, Section &sh){ // ここでElfクラスの情報が必要なので，クラスの参照をコンストラクタの引数として受け取っている．

	// Device クラスのデータカウンタdcの位置をeh.e_shoff に移動する．
	bd.SetDC(bd, eh.E_shoff()); 
	// eh.ShowDC(eh); // 動作確認済み．

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

