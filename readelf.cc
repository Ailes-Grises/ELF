#include<iostream>
#include<unistd.h>
#include"elf64.h"

int main(int argc, char *argv[]){
	std::shared_ptr<Device> bd(new Device("./sampleO0.out")); // ちゃんとファイル名を指定しましょう
	// 必ずElfクラスのインスタンスから定義すること．順番間違えるとエラーになる．
	std::shared_ptr<Elf> Ehdr(new Elf(bd));
	std::shared_ptr<Section> Shdr(new Section(bd, Ehdr));
	std::shared_ptr<Program> Phdr(new Program(bd, Ehdr));

	std::shared_ptr<Symbol> symtab(new Symbol(bd, Shdr, ".symtab"));
	std::shared_ptr<Symbol> dynsym(new Symbol(bd, Shdr, ".dynsym"));

	std::shared_ptr<Dynamic> dynamic(new Dynamic(bd, Shdr));


	if(argc == 1){
		std::cerr<<"Please type option: \n"<<std::endl;
		std::cerr<<"  -h : show ELF header\n  -l : show Program header\n  -S : show Section header\n  -s : show Symbol table\n"<<std::endl;
	}

	int opt;
	while((opt=getopt(argc, argv, "hlSsdz"))!=-1){
		switch(opt){
			case 'h':
				Ehdr->show_ehdr(); break;
			case 'S':
				Shdr->show_shdr(bd, Ehdr); break;
			case 'l':
				Phdr->show_phdr(bd, Ehdr); break;
			case 's':
				dynsym->show_symtab(bd, Shdr);
				symtab->show_symtab(bd, Shdr);
				break;
			case 'd':
				dynamic->show_dynamic(bd, Shdr); break;
			case 'z':
				asm_dumper(bd, Shdr); break;

			default:
				std::cerr<<"Please type option: \n"<<std::endl;
				std::cerr<<"  -h : show ELF header\n  -l : show Program header\n  -S : show Section header\n  -s : show Symbol table\n  -d : show Dynamic section\n"<<std::endl;
				break;
		}
	}

	return 0;
}
