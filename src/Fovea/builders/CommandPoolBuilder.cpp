#include "Fovea/builders/CommandPoolBuilder.hpp"

namespace Fovea{
	void CommandPoolBuilder::setFamily(PhysicalDeviceFamily family){
		this->family = family;
	}

	void CommandPoolBuilder::setFlag(int flags){
		this->flags = flags;
	}
}