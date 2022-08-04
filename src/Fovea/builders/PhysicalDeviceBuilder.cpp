#include "Fovea/builders/PhysicalDeviceBuilder.hpp"

namespace Fovea{
	VkPhysicalDeviceFeatures PhysicalDeviceBuidler::getFeatures(){
		VkPhysicalDeviceFeatures features;
		VkBool32* arr = reinterpret_cast<VkBool32*>(&features);
		
		for (int i=0; i<static_cast<int>(PhysicalDeviceFeature::FEATURES_COUNT); i++){
			arr[i] = requiredFeatures[i];
		}

		return features;
	}

	void PhysicalDeviceBuidler::requireExtension(const char* ext){
		requiredExtensions.push_back(ext);
	}

	void PhysicalDeviceBuidler::requireFamily(PhysicalDeviceFamily family){
		requiredFamilies.set(static_cast<size_t>(family));
	}

	void PhysicalDeviceBuidler::requireFeature(PhysicalDeviceFeature feature){
		requiredFeatures.set(static_cast<size_t>(feature));
	}
}