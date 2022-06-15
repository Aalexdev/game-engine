#include "engine/pch.hpp"
#include "engine/displays/Display.hpp"
#include "engine/displays/SDL2_Display.hpp"

namespace engine{
	Ref<Display> Display::create(const Definition &def){
		return displays::SDL2_Display::create(def);
	}
}