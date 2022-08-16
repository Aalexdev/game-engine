#include "Fovea/core.hpp"
#include "Fovea/DescriptorWriter.hpp"

#include <cassert>

namespace Fovea{
	void DescriptorWriter::initialize(DescriptorSetLayout &setLayout, DescriptorPool &pool){
		this->setLayout = &setLayout;
		this->pool = &pool;
	}

	void DescriptorWriter::writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo){
		assert(setLayout->bindings.count(binding) == 1 && "layout does not contain specified binding");

		auto &bindingDescription = setLayout->bindings[binding];

		assert(bindingDescription.descriptorCount == 1 && "binding single descriptor info, but binding expects multiple");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pBufferInfo = bufferInfo;
		write.descriptorCount = 1;

		writes.push_back(write);
	}

	void DescriptorWriter::writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo){
		assert(setLayout->bindings.count(binding) == 1 && "layout does not contain specified binding");
 
		auto &bindingDescription = setLayout->bindings[binding];

		assert(bindingDescription.descriptorCount == 1 && "binding single descriptor info, but binding expects multiple");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pImageInfo = imageInfo;
		write.descriptorCount = 1;

		writes.push_back(write);
	}

	void DescriptorWriter::writeImages(uint32_t binding, uint32_t imageCount, VkDescriptorImageInfo *imageInfos){
		assert(setLayout->bindings.count(binding) == 1 && "layout does not contain specified binding");
 
		auto &bindingDescription = setLayout->bindings[binding];

		assert(bindingDescription.descriptorCount == 1 && "binding single descriptor info, but binding expects multiple");

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorType = bindingDescription.descriptorType;
		write.dstBinding = binding;
		write.pImageInfo = imageInfos;
		write.descriptorCount = imageCount;

		writes.push_back(write);
	}

	bool DescriptorWriter::build(VkDescriptorSet &set){
		bool success = pool->allocateDescriptor(setLayout->getDescriptorSetLayout(), set);

		if (!success) {
			return false;
		}

		overwrite(set);
  		return true;
	}

	void DescriptorWriter::overwrite(VkDescriptorSet &set){
		for (auto &write : writes) {
			write.dstSet = set;
		}
		vkUpdateDescriptorSets(getInstance().logicalDevice.getDevice(), writes.size(), writes.data(), 0, nullptr);
	}
}