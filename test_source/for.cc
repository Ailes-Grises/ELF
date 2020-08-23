#include<iostream>
using namespace std;

int main(void){
	// for文を使わないでループしてみる．
	int i=0;
	ROOP:
	cout<<"i : "<<i<<endl;
	i++;

	if(i<3){
		goto ROOP;
	}

	return 0;
}
