#include<iostream>
#include<unistd.h>
#include"elf64.h"

int main(int argc, char *argv[]){
	Device bd("./sampleO0.out"); // ちゃんとファイル名を指定しましょう
	// 必ずElfクラスのインスタンスから定義すること．順番間違えるとエラーになる．
	Elf Ehdr(bd);
	Section Shdr(bd, Ehdr);
	Program Phdr(bd, Ehdr);

	Symbol symtab(bd, Shdr, ".symtab");
	Symbol dynsym(bd, Shdr, ".dynsym");


	int opt;
	while((opt=getopt(argc, argv, "hlSs"))!=-1){
		switch(opt){
			case 'h':
				Ehdr.show_ehdr(); break;
			case 'S':
				Shdr.show_shdr(bd, Ehdr); break;
			case 'l':
				Phdr.show_phdr(bd, Ehdr); break;
			case 's':
				dynsym.show_symtab(bd, Shdr);
				symtab.show_symtab(bd, Shdr);
				break;

			default:
				std::cerr<<"Please type option: \n"<<std::endl;
				std::cerr<<"  -h : show ELF header\n  -l : show Program header\n  -S : show Section header\n  -s : show Symbol table\n"<<std::endl;
				break;
		}
	}

	return 0;
}
