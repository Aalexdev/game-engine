#include "core.hpp"

namespace RainDrop{
	Core& getInstance(){
		static Core instance;
		return instance;
	}
}