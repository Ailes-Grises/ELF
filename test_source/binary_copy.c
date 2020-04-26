#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdint.h>
#include<stdlib.h>

// read/write(2) の実用的な運用訓練として，stat(2)で確保した配列にバイナリファイルを読み込み，それを他のファイルに書き込む．

/* 
 * [C言語で書いて思ったこと]
 * 
 * Cだと，クラスとメンバ関数が無いため，配列から8bit読み込む際のユーザ関数の引数の数がかなり増える．
 * また，この際，static宣言を使用したとしても，ユーザ関数内のローカル変数の値が毎回変動してしまうため，バイナリファイルの読み出し開始位置をプログラマが管理しなければならない．(グローバル変数とか使えば出来ないこともないけど美しくない．)
 * やっぱりオブジェクトだけじゃなくて，その基本操作関数であるメンバ関数があるのは凄く便利．管理が楽．
 * 
 */

int main(void){
	int fdr, fdw;
	if((fdr=open("./v6root", O_RDONLY))<0){
		perror("open v6root");
		exit(1);
	}

	if((fdw=open("./ohoho", O_WRONLY))<0){
		perror("open ohoho");
		exit(1);
	}

	// ファイルサイズを調べる
	struct stat r_st;
	if(fstat(fdr, &r_st)<0){
		perror("stat failed");
		exit(1);
	}

	// 領域確保
	uint8_t *p;
	if((p=(uint8_t*)malloc(r_st.st_size*sizeof(uint8_t)))<0){
		printf("malloc failed. /n");
		exit(1);
	}
	printf("r_st.st_size : %d\n", (int)r_st.st_size);

	read(fdr, p, r_st.st_size);
	printf("read complete\n");
	write(fdw, p, r_st.st_size);
	printf("write complete\n");

	free(p);
	close(fdr);
	close(fdw);
	return 0;
}
