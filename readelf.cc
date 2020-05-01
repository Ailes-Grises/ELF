#include<iostream>
#include"elf.h"

int main(int argv, char *argc[]){
	Device bd("./sampleO0.out"); // ちゃんとファイル名を指定しましょう
	Elf Ehdr;

	// コマンドライン引数によって，どのメタデータを表示するのかを判断するべき．
	Ehdr.eh_parser(bd);
	Ehdr.show_ehdr();

	Section Shdr(Ehdr);
	Shdr.sh_parser(bd, Ehdr, Shdr);
	Shdr.show_shdr(bd, Ehdr, Shdr);
	return 0;
}
