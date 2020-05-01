#include<stdio.h>
#include<stdint.h>
int main(void){
	uint8_t chara[2];
	chara[0]='a';
	chara[1]='a'+1;
	fprintf(stderr, "%s\n", chara);
	return 0;
}
