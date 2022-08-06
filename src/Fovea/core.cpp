#include "Fovea/core.hpp"

namespace Fovea{
	Core &getInstance(){
		static Core core;
		return core;
	}

	WaitIdle::~WaitIdle(){
		vkDeviceWaitIdle(getInstance().logicalDevice.getDevice());
	}
}