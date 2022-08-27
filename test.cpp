#include <iostream>

#include "include/RainDrop.hpp"
#include "include/glm/glm.hpp"

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

struct Vertex{
	glm::vec3 pos;
	glm::vec3 color;
};

int main(int argc, char** argv){

	RainDrop::initialize();
	// RainDrop::subscribeEvent("key pressed", &test);

	RainDrop::shutdown();

	return 0;
}
