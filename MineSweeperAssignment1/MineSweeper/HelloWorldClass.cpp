#include "HelloWorldClass.h"
using namespace std;
#include <iostream>

	HelloWorldClass::HelloWorldClass() {
		cout << "Class created" << endl;
		HelloWorldClass::num = 0;
	};

	HelloWorldClass::HelloWorldClass(int paraNum) : num(paraNum) {
	}

	int HelloWorldClass::getNum() {
		return num;
	}

	int HelloWorldClass::setNum(int newNum) {
		HelloWorldClass::num = newNum;
		return HelloWorldClass::num;
	}

