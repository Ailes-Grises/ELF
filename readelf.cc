#include<iostream>
#include"elf.h"

int main(int argv, char *argc[]){
	Elf Ehdr("./sampleO0.out"); // コンストラクタ呼び出しのタイミングで引数を指定しなかったため，Deviceクラスのコンストラクタがいつまで経っても呼び出されず，mallocで確保されていない領域にアクセスしてしまっていたため，セグメント違反でコアダンプが発生していた．

	// コマンドライン引数によって，どのメタデータを表示するのかを判断するべき．
	Ehdr.eh_parser(Ehdr);
	Ehdr.showehdr(Ehdr);
	Section Shdr(Ehdr);
	Shdr.sh_parser(Ehdr, Shdr);
	Shdr.showshdr(Ehdr, Shdr);
	return 0;
}
