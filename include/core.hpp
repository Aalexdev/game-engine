#pragma once

#include <SDL2/SDL.h>
#include "ECS.hpp"
#include "RainDrop.hpp"

namespace RainDrop{
	enum class RenderBuffer{
		None,
		Scene,
		GeneralUsage,
	};

	struct Core{
		SDL_Window* window = nullptr;
		ECS::Coordinator scene;
		bool keyPressed[static_cast<int>(Key::K_MAX)];
		bool buttonPressed[static_cast<int>(MouseButton::MAX)];
		vec2<float> mousePos;
	};

	Core& getInstance();
}