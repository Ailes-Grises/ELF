#include<stdint.h>
#include<iostream>
#include<iomanip>
#include<string>

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
	uint8_t *dc; // data[]上のデータカウンタ． *data に直接アクセスしないほうが良い． このメンバは，他のクラス上から値を変更したい場合があるため，専用の関数を用意する(SetDC(Device &obj))．
	uint8_t *sp; // いわゆるスタックポインタ．
	int Endian; // エンディアンフラグ．まだ未定義．デフォルトでどっちかに設定しといたほうがいいかも．

	public:
	Device(void);
	Device(const char *path);
	~Device(void);
	int Fsize(void);
	void setDC(Device &obj, int Dec_addr); // *dcの指すアドレスを変更する関数．
	uint8_t* getDC(Device &obj); // 他クラス用．dcの指しているアドレス自体を返す関数．
	uint8_t* getDATA(Device &obj); // 他クラス用．dataの指しているアドレス自体を返す関数．
	void showDC(Device &obj); // デバッグ用．
	void setSP(Device &obj, int Dec_addr); // *spの指すアドレスを変更する関数．
	uint8_t get8bit(Device &obj); // 以下の get*bit() はエンディアン補正済み．
	uint16_t get16bit(Device &obj);
	uint32_t get32bit(Device &obj);
	uint64_t get64bit(Device &obj);
	uint8_t getChar(Device &obj); // 内部で*spを使用．バイトオフセットは考慮されていない．
};

// デバッグ用の便利関数．
unsigned int Dec_frombin(std::string &bin); // 2進数 -> 10進数
std::string Bin_fromdec(unsigned int dec); // 10進数 -> 2進数
std::string Hex_fromdec(unsigned int dec); // 10進数 -> 16進数
unsigned int Dec_fromhex(std::string &hex); // 16進数 -> 10進数
std::string Hex_frombin(std::string &bin); // 2進数 -> 16進数
std::string  Bin_fromhex(std::string &hex); // 16進数 -> 2進数
