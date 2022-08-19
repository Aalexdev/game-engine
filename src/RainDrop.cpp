#include "RainDrop.hpp"
#include "horreum/Horreum.hpp"
#include "Fovea.h"
#include "Gramophone.hpp"
#include "Odin.hpp"

namespace RainDrop{
	void RD_API initialize(){
		Horreum::initialize();
		// FoveaInitialize();
		Gramophone::initialize();
		Odin::initialize();
	}
}