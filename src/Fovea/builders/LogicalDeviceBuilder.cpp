#include "Fovea/builders/LogicalDeviceBuilder.hpp"

namespace Fovea{
	void LogicalDeviceBuilder::requireExtension(const char *extension){
		requiredExtensions.push_back(extension);
	}

	void LogicalDeviceBuilder::requireQueue(PhysicalDeviceFamily family){
		requiredQueues.set(static_cast<size_t>(family));
	}

	void LogicalDeviceBuilder::setQueuePriority(PhysicalDeviceFamily family, float priority){
		queuePriorities[static_cast<size_t>(family)] = priority;
	}
}