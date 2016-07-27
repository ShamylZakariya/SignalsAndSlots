//
//  main.cpp
//  SignalsAndSlots
//
//  Created by Shamyl Zakariya on 7/26/16.
//  Copyright © 2016 Shamyl Zakariya. All rights reserved.
//

#include <iostream>
#include <cassert>

#include "signals.h"

namespace {

	int free_function_state = 0;

}

void free_function_zero() {
	free_function_state++;
}

void free_function_one(int v) {
	free_function_state += v;
}

void free_function_two(int v0, int v1) {
	free_function_state += v0 + v1;
}


class Callee : public signals::receiver {
public:

	Callee(){}

	void method_zero() {
		state++;
	}

	void method_one(int v) {
		state += v;
	}

	void method_two(int v0, int v1) {
		state += v0 +v1;
	}


public:

	static int state;

};

int Callee::state = 0;



int main(int argc, const char * argv[]) {


	Callee c0, c1;

	// test zero args
	signals::signal<void()> signalZero;
	signalZero.connect(free_function_zero);
	signalZero.connect(&c0, &Callee::method_zero);
	signalZero.connect(&c1, &Callee::method_zero);

	free_function_state = 0;
	Callee::state = 0;
	signalZero();

	assert(free_function_state == 1);
	assert(Callee::state == 2);

	signalZero.disconnect(free_function_zero);
	signalZero.disconnect(&c0);
	signalZero.disconnect(&c1);

	signalZero();

	// state should not have been modified
	assert(free_function_state == 1);
	assert(Callee::state == 2);


	// test 1 argument
	signals::signal<void(int)> signalOne;
	signalOne.connect(free_function_one);
	signalOne.connect(&c0, &Callee::method_one);
	signalOne.connect(&c1,&Callee::method_one);

	free_function_state = 0;
	Callee::state = 0;
	signalOne(1);

	assert(free_function_state == 1);
	assert(Callee::state == 2);

	// test 2 arguments

	signals::signal<void(int,int)> signalTwo;
	signalTwo.connect(free_function_two);
	signalTwo.connect(&c0, &Callee::method_two);
	signalTwo.connect(&c1, &Callee::method_two);

	free_function_state = 0;
	Callee::state = 0;
	signalTwo(1,2);

	assert(free_function_state == 3);
	assert(Callee::state == 6);

	// test automatic disconnection
	{

		Callee c2;
		signalOne.connect(&c2, &Callee::method_one);

		free_function_state = 0;
		Callee::state = 0;
		signalOne(5);

		assert(free_function_state == 5);
		assert(Callee::state == 15); // c0, c1 & c2 will have added 5 each
	}

	free_function_state = 0;
	Callee::state = 0;
	signalOne(5);

	assert(free_function_state == 5);
	assert(Callee::state == 10); // c0, c1 will have added 5 each, c3 has been disconnected



	return 0;
}
