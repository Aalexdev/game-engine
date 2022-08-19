#include <iostream>

#include "include/RainDrop.hpp"

int main(int argc, char** argv){

	RainDrop::initialize();
	RainDrop::run();
	RainDrop::shutdown();

	return 0;
}
