#pragma once

#include <SDL2/SDL.h>

namespace RainDrop{
	struct Core{
		SDL_Window* window = nullptr;
	};

	Core& getInstance();
}