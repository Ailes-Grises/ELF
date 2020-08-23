#include<iostream>
#include<vector>
#include<stdint.h>

using std::cout;
using std::cerr;
using std::endl;

typedef struct{
	uint32_t p_type;
	uint32_t p_flags;
} Elf64_Phdr;

int main(void){
	std::vector<Elf64_Phdr> phdr(10);
	// Elf64_Phdr phdr[10];
	phdr[0].p_type=(uint32_t)1;
	phdr[0].p_flags=(uint32_t)1;
	cout<<"phdr[0].p_type : "<<phdr[0].p_type<<endl;
	cout<<"phdr[0].p_flags : "<<phdr[0].p_flags<<endl;

	return 0;
}
