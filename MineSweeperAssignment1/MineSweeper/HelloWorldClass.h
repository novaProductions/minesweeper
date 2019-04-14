#ifndef HELLOWORLDCLASS_H
#define HELLOWORLDCLASS_H

class HelloWorldClass
{
	public:
		HelloWorldClass();
		HelloWorldClass(int paraNum);
		int getNum();
		int setNum(int newNum);
	private:
		int num;
};

#endif

