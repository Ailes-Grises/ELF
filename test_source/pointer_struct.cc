#include<iostream>

using std::cout;
using std::endl;

typedef struct{
	char *Name;
	int Age;
} Person;

int main(void){
	Person hoge, *Naoto; // あえてポインタを宣言し，これを介してデータにアクセスしてみる．
	Naoto=&hoge; // こうすれば，今後hogeと*Naoto は同じものとなる．

	Naoto->Name = "Naoto";
	Naoto->Age = 23;

	cout<<"Name : "<<Naoto->Name<<endl;
	cout<<"Age : "<<Naoto->Age<<endl;

	return 0;
}
