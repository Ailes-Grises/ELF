#include<iostream>
#include"device.h"
#define N 1024

int main(void){
	Device obj("./sample.out");
	/*
	uint8_t ch;
	std::cout<<"size : "<<obj.Fsize()<<std::endl;

	for(int i=0;i<EHDR;i++){
		if(i!=0 && i%2==0) fprintf(stderr, " ");
		if(i!=0 && i%16==0) fprintf(stderr, "\n");
		ch=obj.Data(i);
		fprintf(stderr, "%x", ch);
	}
	*/
	return 0;
}
