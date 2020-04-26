#include<stdio.h>
#include<stdint.h>

int main(void){
	uint8_t del=0x7f;
	uint8_t E=0x45;
	uint16_t X;
	X=del;
	fprintf(stderr, "X : %x\n", X);
	X=(X<<8)|E;
	fprintf(stderr, "X : %x\n", X);

	return 0;
}
