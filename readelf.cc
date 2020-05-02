#include<iostream>
#include"elf64.h"

int main(int argv, char *argc[]){
	Device bd("./sampleO0.out"); // ちゃんとファイル名を指定しましょう
	Elf Ehdr;

	// コマンドライン引数によって，どのメタデータを表示するのかを判断するべき．
	Ehdr.eh_parser(bd);
	Ehdr.show_ehdr();

	Section Shdr(Ehdr);
	Shdr.sh_parser(bd, Ehdr, Shdr);
	//Shdr.show_shdr(bd, Ehdr, Shdr);

	Program Phdr(Ehdr);
	Phdr.ph_parser(bd, Ehdr, Phdr);
	//Phdr.show_phdr(bd, Ehdr, Phdr);
	return 0;
}
