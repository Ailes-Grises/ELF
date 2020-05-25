#include<iostream>
#include<string>
#include<unordered_map>
#define N 5
using std::cout;
using std::endl;

int main(void){
	std::string* arr= new std::string[5];

	for(int j=0;j<3;j++){
		arr[0]+='a';
		arr[1]+='b';
		arr[2]+='c';
		arr[3]+='d';
		arr[4]+='e';
	}
	for(int i=0;i<5;i++) cout<<"arr["<<i<<"] : "<<arr[i]<<endl;

	std::unordered_map<std::string, int> hash;
	hash[arr[0]]=0;
	hash[arr[1]]=1;
	hash[arr[2]]=2;
	hash[arr[3]]=3;
	hash[arr[4]]=4;
	for(int i=0;i<5;i++) cout<<"key of arr["<<i<<"] : "<<hash[arr[i]]<<endl;

	delete [] arr;
	return 0;
}

