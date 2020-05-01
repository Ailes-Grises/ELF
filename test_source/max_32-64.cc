#include<iostream>
#include<stdint.h>
int main(void){
	uint32_t x=4294967295;
	uint64_t y=18446744073709551615;
	std::cout<<"x : "<<x<<std::endl;
	std::cout<<"y : "<<y<<std::endl;

	uint32_t bin=0b11111111111111111111111111111111;
	std::cout<<"bin : "<<bin<<std::endl;

	//unsigned int z=1;
	//for(int i=0;i<31;i++) z=1|z<<1;
	//fprintf(stderr, "z : %ld\n",z);
	return 0;
}
