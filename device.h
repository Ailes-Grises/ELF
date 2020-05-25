#include<stdint.h>
#include<iostream>
#include<iomanip>
#include<string>
using std::cout;
using std::endl;
using std::cerr;

#define LE 0 // リトルエンディアン
#define BE 1 // ビッグエンディアン

#define hexformat(wd) std::hex<<std::setfill('0')<<std::setw(wd)
// uint8_t型は本来unsigned char なので，この関数を使ってuint8_t型の16進数を16進表示させるには，直前に(unsigned int)とかで整数型にキャストする必要がある．


// 対象バイナリファイルを読み込み，Device::data に格納するクラス．
// ELFファイルのフォーマット理解が目的．

class Device{
	private:
	uint8_t *data; // バイナリデータを格納．
	uint8_t *dc; // data[]上のデータカウンタ． *data に直接アクセスしないほうが良い． このメンバは，他のクラス上から値を変更したい場合があるため，専用の関数を用意する(SetDC(Device &obj))．
	uint8_t *sp; // スタックポインタ．*dataを動かしたくない時はこっちを使う．
	int Endian; // エンディアンフラグ．まだ未定義．デフォルトでどっちかに設定しといたほうがいいかも．

	public:
	Device(const char *path);
	~Device(void);
	void setDC(int addr); // *dcの指すアドレスを変更する関数．
	uint8_t* getDC(void); // 他クラス用．dcの指しているアドレス自体を返す関数．
	uint8_t* getDATA(void); // 他クラス用．dataの指しているアドレス自体を返す関数．
	void showDC(void); // デバッグ用．
	void setSP(int addr); // *spの指すアドレスを変更する関数．

	uint8_t get8bit(void); // 以下の get*bit() はエンディアン補正済み．
	uint16_t get16bit(void);
	uint32_t get32bit(void);
	uint64_t get64bit(void);
	uint8_t getChar(void); // 内部で*spを使用．バイトオフセットは考慮されていない．
};

// デバッグ用の便利関数．
unsigned int Dec_frombin(std::string &bin); // 2進数 -> 10進数
std::string Bin_fromdec(unsigned int dec); // 10進数 -> 2進数
std::string Hex_fromdec(unsigned int dec); // 10進数 -> 16進数
unsigned int Dec_fromhex(std::string &hex); // 16進数 -> 10進数
std::string Hex_frombin(std::string &bin); // 2進数 -> 16進数
std::string  Bin_fromhex(std::string &hex); // 16進数 -> 2進数
