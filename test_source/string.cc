#include<iostream>
#include<string>

int main(void){
	// 1. 基本的な扱い方
	std::string str;
	str+='a';
	std::cout<<"str : "<<str<<std::endl;
	str+='b';
	str+='c';
	std::cout<<"str : "<<str<<std::endl;


	// 2. std::stringクラス型の配列も可能．
	std::string name[3];
	name[0]+='d';
	name[0]+='e';

	name[1]+='f';
	name[1]+='g';

	name[2]+='g';
	name[2]+='g';
	name[2]+='g';

	std::cout<<"name[0] : "<<name[0]<<std::endl;
	std::cout<<"name[1] : "<<name[1]<<std::endl;
	std::cout<<"name[2] : "<<name[2]<<std::endl;

	// 3. newしてstd::stringクラス型の配列を作成することも可能(!?)
	std::string *data;
	data=new std::string[5];
	data[0]+="first";
	data[1]+="second";
	data[2]+="Third";
	std::cout<<"data[0] : "<<data[0]<<std::endl;
	std::cout<<"data[1] : "<<data[1]<<std::endl;
	std::cout<<"data[2] : "<<data[2]<<std::endl;

	std::cout<<"data[0][1]"<<data[0][1]<<std::endl;
	delete [] data;

	return 0;
}
