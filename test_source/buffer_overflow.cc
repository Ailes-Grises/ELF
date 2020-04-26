#include<iostream>
#include<stdlib.h>
#include<stdint.h>
#define N 300

// uint8_t *dc は，「指している先に格納されているデータの範囲が0~255」であるが，アドレスの範囲に関してはint やdouble と何ら変わりがなく，特に制限はない．よって，「dc+=260」みたいな場合にも，問題なくデータを指すことができる．

int main(void){
	uint8_t *data, *dc;
	data=(uint8_t*)malloc(N*sizeof(uint8_t));
	for(int i=0,j=1;i<N;i++){
		if(i<256){
			data[i]=i;
		}else if(i>=256){
			data[i]=i-2*j;
			j++;
		}
		//fprintf(stderr, "i : %d (0x%p)\n", *(data+i),data+i);
	}

	dc=data;
	for(int i=0;i<N;i++,dc+=1) fprintf(stderr, "*(dc+%d) : %d\n", i, *dc);

	free(data);
	return 0;
}
