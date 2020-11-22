// 何故か謎のエラーが出る...
// リンカエラーのせいでデバッグも出来ない...orz
#include<iostream>
using namespace std;

class hoge{
	private:
		static int x;
	public:
		hoge(void){
		};
		~hoge(void){};
		static void st_print(void){
			hoge::x = 2;
			cout << "x : " << x << endl;
			return;
		};
		void print(void){
			cout << "x : " << x << endl;
			return;
		};
};

int main(void){
	hoge::st_print();
	return 0;
}

