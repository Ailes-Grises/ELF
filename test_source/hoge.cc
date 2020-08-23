#include<iostream>
#include<memory>
#include<string>

using namespace std;

class foo;

class hoge{
	private:
	int key;
	string value;

	public:
	hoge(void){
	};
	hoge(int key, string value){
		this->key = key;
		this->value = value;
	};
	~hoge(void){
	};
	void key_to_value(int key){
		cout << value << endl;
		return;
	};
	void value_to_key(string value){
		cout << key << endl;
		return;
	};
	void get_foo(shared_ptr<foo> Foo){
		// Foo->show();
		return;
	};
};

class foo{
	private:
	int x;
	public:
	foo(void){
	};
	foo(int x){
		this->x=x;
	};
	~foo(void){
	};
	void show(void){
		cout<<"x : "<<x<<endl;
		return;
	};
	void get_hoge(shared_ptr<hoge> Hoge){
		Hoge->key_to_value(23);
		return;
	};
};

int main(void){
	// hoge X(23, "Naoto");
	shared_ptr<hoge> Hoge(new hoge(23, "Naoto"));
	shared_ptr<foo> Foo(new foo(10));
	Foo->get_hoge(Hoge);
	return 0;
}
