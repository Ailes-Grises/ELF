#include<iostream>
#include<string>

std::string Bin_fromdec(unsigned int dec){
	if(dec==0) return std::string("0");

	std::string bin;
	while(dec!=0){
		if((dec%2)==0){
			bin.insert(bin.begin(), '0');
		}else{
			bin.insert(bin.begin(), '1');
		}
		dec/=2;
	}
	return bin;
};

int main(void){
	std::string bin;
	unsigned int dec=11; // (1011)

	bin=Bin_fromdec(dec);
	std::cout<<"bin : "<<bin<<std::endl;
	return 0;
}

