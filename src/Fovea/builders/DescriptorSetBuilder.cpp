#include "Fovea/builders/DescriptorSetBuilder.hpp"

namespace Fovea{
	void DescriptorSetBuilder::setDescriptors(const std::vector<Descriptor> &descriptors){
		this->descriptors = descriptors;
	}

	void DescriptorSetBuilder::setDescriptorSetCount(uint32_t count){
		descriptorSetCount = count;
	}
}