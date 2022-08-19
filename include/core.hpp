#pragma once

#include <SDL2/SDL.h>
#include "ECS.hpp"

namespace RainDrop{
	struct Core{
		SDL_Window* window = nullptr;
		ECS::Coordinator scene;
	};

	Core& getInstance();
}