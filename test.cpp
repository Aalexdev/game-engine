#include <iostream>

#include "include/RainDrop.hpp"

bool test(void *ptr){
	RainDrop::Key k;
	bool repeat;

	RainDrop::convertFromVoid(ptr, k, repeat);

	if (k == RainDrop::Key::K_A){
		printf("aaa, %d\n", repeat);
		RainDrop::triggerEvent("key pressed", RainDrop::Key::K_B, false);
	}

	if (k == RainDrop::Key::K_B){
		printf("bbb\n");
	}

	return false;
}


int main(int argc, char** argv){

	RainDrop::initialize();
	RainDrop::subscribeEvent("key pressed", &test);

	RainDrop::run();
	RainDrop::shutdown();

	return 0;
}
