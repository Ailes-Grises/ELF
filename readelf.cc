#include<iostream>
#include<unistd.h>
#include"elf64.h"

int main(int argc, char *argv[]){
	Device bd("./sampleO0.out"); // ちゃんとファイル名を指定しましょう
	// 必ずElfクラスのインスタンスから定義すること．順番間違えるとエラーになる．
	Elf Ehdr(bd);
	Section Shdr(bd, Ehdr);
	Program Phdr(bd, Ehdr);


	int opt;
	while((opt=getopt(argc, argv, "hlS"))!=-1){
		switch(opt){
			case 'h':
	Ehdr.show_ehdr(); break;
			case 'l':
	Shdr.show_shdr(bd, Ehdr); break;
			case 'S':
	Phdr.show_phdr(bd, Ehdr); break;
			default:
				std::cerr<<"Please type option: \n"<<std::endl;
				std::cerr<<"  -h : show ELF header\n  -l : show program header\n  -S : show Section header\n"<<std::endl;
				break;
		}
	}

	return 0;
}
