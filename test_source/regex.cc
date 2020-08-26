/* C++ による正規表現の利用方法 */
#include<iostream>
#include<string>
#include<regex>

using std::cout;
using std::endl;

int main(void){
	std::string str="DT_AAA and DT_BBB"; // 対象文字列
	// std::regex re("(?<=DT_)([A-Z][A-Z][A-Z])"); // 正規表現のパターンを保持するクラスのインスタンス生成
	std::regex re("(DT_)(\\w\\w\\w)");
	std::smatch results; // string 型の文字列に対するマッチング結果を格納するのがsmatch 型．"()" によってグループ化された文字列はサブマッチとして配列に格納される．

	/* 
	 * 上の例だと，results[0] = "DT_AAA and DT_BBB"
	 *             results[1] = "AAA"
	 *             results[1] = "BBB"
	 *  となる．
	 *
	 */

	std::regex_search(str, results, re);

	cout << "results[0] : " << results[0].str() << endl;
	cout << "results[1] : " << results[1].str() << endl;
	cout << "results[2] : " << results[2].str() << endl;

	return 0;
}
