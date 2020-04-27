#include<stdint.h>
#include<iostream>
#include<iomanip>

#define EHDR 64
#define LE 0 // リトルエンディアン
#define BE 1 // ビッグエンディアン

#define hexformat(wd) std::hex<<std::setfill('0')<<std::setw(wd)
// uint8_t型は本来unsigned char なので，この関数を使ってuint8_t型の16進数を16進表示させるには，直前に(unsigned int)とかで整数型にキャストする必要がある．


// 対象バイナリファイルを読み込み，Device::data に格納するクラス．
// ELFファイルのフォーマット理解が目的．

class Device{
	private:
	uint8_t *data; // バイナリデータを格納．
	int fsize;
	uint8_t *dc; //data[]上のデータカウンタ． *data に直接アクセスしないほうが良い． このメンバは，他のクラス上から値を変更したい場合があるため，専用の関数を用意する(SetDC(Device &obj))．
	int Endian; //エンディアンフラグ．まだ未定義．デフォルトでどっちかに設定しといたほうがいいかも．
	public:
	Device(void);
	Device(const char *path);
	~Device(void);
	int Fsize(void);
	void SetDC(Device &obj, int Dec_addr); // *dcの指すアドレスを変更する関数．
	void ShowDC(Device &obj); // デバッグ用．
	uint8_t get8bit(Device &obj); // 以下の get*bit() はエンディアン補正済み．
	uint16_t get16bit(Device &obj);
	uint32_t get32bit(Device &obj);
	uint64_t get64bit(Device &obj);
};
