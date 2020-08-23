#include<iostream>
#include<memory>
#include<stdint.h>

using std::cout;
using std::endl;

	/* 
	 * [smart pointer]
	 *
	 * #include<memory>
	 * 
	 * std::unique_ptr<型> ポインタ変数名
	 * 				or
	 * std::unique_ptr<型> ポインタ変数名(new 型(数))  //※ 変数名の直後に"()"をつけると変数を初期化できる
	 * 
	 */

class Data{
	private:
	uint8_t *data;
	public:
	Data(int num){
		cout<<"constractor!"<<endl;
		data=new uint8_t[num];
	};
	~Data(void){
		// delete [] data;
		cout<<"delete!"<<endl;
	};
	void hello(void){
		cout<<"Hello!"<<endl;
	};
};

int main(void){
	// data クラスの(スマート)ポインタbd を宣言して，make_shared<data> でdata クラスのコンストラクタを呼び出している(?)
	// std::shared_ptr<data> bd=std::make_shared<data>(3); // 初期化の際には極力std::make_shared<T> を利用すべき．

	// data クラスのスマートポインタbd を宣言して，() で初期化．その際，new を使ってクラスのインスタンスを生成している．
	std::shared_ptr<Data> bd(new Data(3)); // この例の方がまだ分かりやすいかも．

	// クラスのインスタンスはこれでも作成可能!
	// Data *bd(new Data(3));
	// bd->hello();

	bd->hello();

	// cout<<"何らかのエラー発生"<<endl;
	// exit(1);

	return 0;
}

/*
 * スマートポインタの基本的な使い方
 *
 * そもそも，スマートポインタが欲しい時ってどういう状況なのかというと，
 *
 * 1. new で確保した配列の廃棄し忘れが怖い
 * 2. クラスのコンストラクタでnew で確保した領域がエラーによって破棄されないのが怖い
 *
 * あたりが妥当な理由だろう．
 * 特に，2 の状況はかなり深刻なメモリーリークを引き起こす可能性がある．
 *
 * そこで，スマートポインタの登場である．
 * まず，コンストラクタ内部の領域確保自体は，普通のポインタとnew で普通に行う．
 * しかし，このクラスのインスタンスを生成する際にはちょっとした工夫をする．
 * インスタンス生成の際，本当に普通のインスタンスを生成するのではなくて，そのクラスのポインタ型のインスタンスを生成する．
 * そして，このクラスのポインタ型インスタンスを，生のポインタでは無くて，スマートポインタとして宣言する．
 * 要するに，このクラスのインスタンスが最後にちゃんと破棄されればそれで問題ないのだから，こうすればインスタンスの寿命と同時に領域開放も行われる．
 * これが，クラス内部で確保した領域をセキュアに扱う方法である．
 * もちろん，生成されるクラスのインスタンスは(スマート)ポインタなので，各関数にアクセスする際には"->" を使用する必要はある．
 */
