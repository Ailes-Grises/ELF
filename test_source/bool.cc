#include<iostream>
#define LE 0 // リトルエンディアン
#define BE 1 // ビッグエンディアン

using namespace std;

int main(void){
	bool Endian;

	Endian=LE;
	cout<<"Endian : "<<Endian<<endl;

	Endian=BE;
	cout<<"Endian : "<<Endian<<endl;

	return 0;
}
