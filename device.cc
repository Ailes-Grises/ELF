#include<iostream>
#include<cstdlib>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdint.h>
#include"device.h"

Device::Device(void){
};

Device::Device(const char *path):Endian(LE){
	int fd;
	struct stat st;
	if((fd=open(path, O_RDONLY))<0){
		perror("device open");
		exit(1);
	}
	fstat(fd, &st);
	fsize=st.st_size;
	if((data=new uint8_t[fsize])==NULL){
		std::cerr<<"new failed. "<<std::endl;
		exit(1);
	}
	read(fd, data, fsize);
	dc=data;
	sp=data;
	// delete はしない．
	close(fd);
};

Device::~Device(void){
	delete [] data;
};

int Device::Fsize(void){
	return fsize;
};

void Device::setDC(Device &obj, int Dec_addr){
	// 10進数のファイルオフセット(アドレス)を受け取り，*dc の値を変更する．
	dc=data+Dec_addr*sizeof(uint8_t);
	return;
};

uint8_t* Device::getDC(Device &obj){
	return dc;
};

uint8_t* Device::getDATA(Device &obj){
	return data;
};

void Device::showDC(Device &obj){
	std::cout<<"*dc : "<<hexformat(2)<<(unsigned int)*dc<<std::endl;
};

void Device::setSP(Device &obj, int Dec_addr){
	// 10進数のファイルオフセット(アドレス)を受け取り，*sp の値を変更する．
	sp=data+Dec_addr*sizeof(uint8_t);
	return;
};

uint8_t Device::get8bit(Device &obj){
	uint8_t *temp=dc;
	dc+=sizeof(uint8_t);
	return *(temp); // 読み込んだらdcを一つ進める．
	// dcのインデックスがめちゃくちゃになってしまうため，配列から1Byte読み込むときには必ずこの関数を使用すること．
	// この関数はエンディアンを意識していない．理由は，この関数がバイナリの冒頭以外で呼ばれる事が恐らく無いからである．
	// リトルエンディアンに従う領域でこの関数を複数回呼ぶのはマズい．そういう時は別の関数を作ったほうが良い．
};

uint16_t Device::get16bit(Device &obj){
	uint8_t *temp=dc;
	dc+=2; //先にカウンタだけは次のデータ読み出しの位置に移動しておいてある．だから+=1ではなくて+=2である．
	//dc+=2*sizeof(uint8_t);
	if(Endian==LE){
		//fprintf(stderr, "*temp : %2x\n", *temp);
		//fprintf(stderr, "*(temp+sizeof(uint8_t)) : %2x\n", *(temp+sizeof(uint8_t)));

		//return ((uint16_t)*(temp++))<<8|(uint16_t)*(temp); // 失敗例．temp++ではだめ．ちゃんとsizeof(uint8_t)分アドレス移動しないとおかしくなっちゃうよ
		return ((uint16_t)*(temp+sizeof(uint8_t)))<<8|(uint16_t)*(temp); //リトルエンディアン
	}
	else{
		return ((uint16_t)*temp)<<8|(uint16_t)*(temp+sizeof(uint8_t)); //ビッグエンディアン
	}
};

uint32_t Device::get32bit(Device &obj){
	if(Endian==LE){
		return obj.get16bit(obj)|((uint32_t)obj.get16bit(obj)<<16);
	}
	else{
		return ((uint32_t)obj.get16bit(obj))<<16|(uint32_t)obj.get16bit(obj);
	}
};

uint64_t Device::get64bit(Device &obj){
	if(Endian==LE){
		return (uint64_t)obj.get32bit(obj)|((uint64_t)obj.get32bit(obj)<<32);
	}
	else{
		return ((uint64_t)obj.get32bit(obj))<<32|(uint64_t)obj.get32bit(obj);
	}
};

uint8_t Device::getChar(Device &obj){
	uint8_t *temp=sp;
	sp+=sizeof(uint8_t);
	return *(temp);
};

// getShort() やgetLong() を作成する場合には，get16bit() などの構造がそのまま使えるはず．


// 2進数 -> 16進数
std::string Hex_frombin(std::string &bin){
	unsigned int dec=Dec_frombin(bin);
	return Hex_fromdec(dec);
};

// 16進数 -> 2進数
std::string Bin_fromhex(std::string &hex){
	unsigned int dec=Dec_fromhex(hex);
	return Bin_fromdec(dec);
};

// 2進数 -> 10進数
unsigned int Dec_frombin(std::string &bin){
	unsigned int dec=0;
	// 2進数を，上位の桁から数えるアルゴリズム．イテレータが最上桁からスタートする．めっちゃ賢いなこれ．
	for(int i=0;i<(int)bin.size();i++){
		switch(bin[i]){
			case '0':
				dec*=2;
				break;
			case '1':
				dec=dec*2+1;
				break;
		}
	}
	return dec;
};


/* <シフト演算による2進数の算出方法>
 *
 * decをひたすら2で割って剰余を見続ける行為(2進変換の基本的なやり方です) == decが偶数かどうかをひたすら確かめ続ける行為
 * 性質1. ある数字が偶数ならば，2進表記の際，最下桁の数字が0になる -> よって，2進数の(0001)とdecの論理積の結果が0ならばdecは偶数(最下ビットだけ比較している状態)
 * つまり，$((dec & 1)) の結果が0ならば偶数なので，余りは0だからstd::stringのコンテナの冒頭に0を代入すればよい -> 逐次的にシフトさせれば順番も整う
 * 最後に，decの値を剰余を考慮せずに2で割る必要がある
 * 性質2. ある数字を右に1bitシフトさせると，その値は$((元の数字/2)) の商となる
 * つまり，最後に$((dec>>=1))してやれば，dec/2と同じ効果が得られる
 */

// 10進数 -> 2進数
std::string Bin_fromdec(unsigned int dec){
	// 0だったらそのまんま返す
	if(dec==0) return std::string("0");

	std::string bin;
	while(dec!=0){
		if((dec & 1)==0){ // if((dec%2)==0){
			bin.insert(bin.begin(), '0'); // 偶数
		}else{
			bin.insert(bin.begin(), '1'); // 奇数
		}
		dec>>=1; // dec/=2;
	}
	return bin;
};

// 10進数 -> 16進数
std::string Hex_fromdec(unsigned int dec){
	if(dec==0) return std::string("0");

	std::string hex;
	int mod;
	while(dec!=0){
		// 16進数の性質(0x0f=0x00001111, 0xf0=11110000)を利用して，4bitずつ読み出す．
		if((mod=dec&15)<10){ // まずは最下桁4bitを読み出す
			hex.insert(hex.begin(), mod);
		}else{
			hex.insert(hex.begin(), 'a'+mod-10);
		}
		dec>>=4; // 上位ビットを降ろす & 用済みの桁には退去してもらう
	}
	return hex;
};

// 16進数 -> 10進数
unsigned int Dec_fromhex(std::string &hex){
	unsigned int dec=0;
	for(int i=0;i<hex.size();i++){
		unsigned int temp;
		if('0'<=hex[i] && hex[i]<='9'){
			temp=hex[i]-'0'; // ascii文字コードの演算を行っている．char('1'-'0')=int(49-48)=1
		}else if('a'<=hex[i] && hex[i]<='f'){
			temp=hex[i]-'a'+10;
		}else if('A'<=hex[i] && hex[i]<='F'){
			temp=hex[i]-'A'+10;
		}else{
			continue; // 16進数以外の文字ならスキップ
		}
		dec=dec*16+temp;
	}
	return dec;
};
