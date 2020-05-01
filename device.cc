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
