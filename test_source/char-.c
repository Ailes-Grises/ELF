#include<stdio.h>
int main(void){
	fprintf(stderr, "'a' : %d\n", 'a');
	fprintf(stderr, "'b' : %d\n", 'b');
	fprintf(stderr, "'b'-'a' : %d\n", 'b'-'a');
	return 0;
}
