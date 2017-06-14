#include <iostream>


class test1
{
	public:
	int get(){return a;};
//	private:
	int  a;
	int  b;
};

class test2
{
	public:
        char get(){return a;};
  //      private:
	char a;
};

class cls_ptr
{
	public:
#ifdef AAA
	int get(){return ((test1*)cls)->get();};
#endif
#ifdef BBB
	char get(){return ((test2*)cls)->get();};
#endif
	void *cls;
};

int main()
{
	cls_ptr good = cls_ptr();
	test2 bad = test2();
	bad.a = 'x';

	good.cls = &bad;
	std::cout << "get " << good.get() << std::endl; 
}


