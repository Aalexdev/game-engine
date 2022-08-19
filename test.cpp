#include <iostream>

#include "include/RainDrop.hpp"

int main(int argc, char** argv){

	RainDrop::initialize();

	_sleep(5000);

	RainDrop::shutdown();

	return 0;
}
