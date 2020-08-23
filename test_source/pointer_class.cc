#include<iostream>
#include<string>
using namespace std;

// タイマーを作成し，これを二人のランナーに渡して徒競走をする．

class Timer{
	private:
	int time;
	public:
	Timer(void){
	};
	~Timer(void){
	};
	void Start(int time){
		this->time=time;
	};
	void Stop(void){
		cout<<"time : "<<this->time<<endl;
	};
};

class Runner{
	private:
	string name;
	int age;
	Timer *tm;
	public:
	Runner(void){
	};
	Runner(string name, int age) : name(name), age(age){
	};
	~Runner(void){
	};
	void getTimer(Timer &timer){
		tm=&timer;
	};
	void startTimer(int time){
		tm->Start(time);
	};
	void stopTimer(void){
		tm->Stop();
	};
};

int main(void){
	Timer X, Y;
	// Runner a("A", 15), b("B", 16);
	Runner *A(new Runner("A", 15)), *B(new Runner("B", 16));

	// A=&a;
	// B=&b;

	A->getTimer(X);
	B->getTimer(Y);
	A->startTimer(10);
	B->startTimer(15);
	A->stopTimer();
	B->stopTimer();

	return 0;
}

