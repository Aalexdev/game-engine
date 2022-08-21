#include <iostream>

#include "include/RainDrop.hpp"

bool test(void *ptr){
	RainDrop::Key* k = static_cast<RainDrop::Key*>(ptr);
	if (*k == RainDrop::Key::K_A){
		printf("aaa\n");
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
