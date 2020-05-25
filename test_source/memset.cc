#include<string>
#include<string.h>
#include<iostream>
#include<elf.h>
using std::cout;
using std::endl;

int main(void){
	Elf64_Shdr *shdr;
	shdr=new Elf64_Shdr[3];
	cout<<"sizeof(shdr) : "<<sizeof(shdr[0])<<endl;

	for(int i=0;i<3;i++){
		memset(&shdr[i], 0, sizeof(shdr[i]));
		cout<<shdr[i].sh_name<<endl;
		cout<<shdr[i].sh_type<<endl;
		cout<<shdr[i].sh_flags<<endl;
		cout<<shdr[i].sh_addr<<endl;
		cout<<shdr[i].sh_offset<<endl;
		cout<<shdr[i].sh_size<<endl;
		cout<<shdr[i].sh_link<<endl;
		cout<<shdr[i].sh_info<<endl;
		cout<<shdr[i].sh_addralign<<endl;
		cout<<shdr[i].sh_entsize<<endl;
	}
	delete [] shdr;
	return 0;
}
