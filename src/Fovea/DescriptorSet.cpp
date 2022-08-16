#include "Fovea/DescriptorSet.hpp"
#include "Fovea/core.hpp"
#include "Fovea/DescriptorWriter.hpp"

#include <unordered_map>
#include <stdexcept>

namespace Fovea{

	static inline VkDeviceSize getAlignment(VkDeviceSize instanceSize, VkDeviceSize minOffsetAlignment){
		if (minOffsetAlignment > 0) {
			return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
		}
		return instanceSize;
	}

	DescriptorSet::~DescriptorSet(){
		delete[] sets;
		delete[] buffers;
		if (buffer) delete[] buffer;
	}

	void DescriptorSet::initialize(DescriptorSetBuilder &builder){
		sets = new VkDescriptorSet[builder.descriptorSetCount];
		buffers = new BufferRange[builder.descriptorSetCount];

		descriptorSetCount = builder.descriptorSetCount;

		createPool(builder);
		createSetLayout(builder);
		createBuffers(builder);
		createDescriptorSets(builder);
	}

	void DescriptorSet::createPool(DescriptorSetBuilder &builder){
		DescriptorPoolBuilder poolBuilder;

		poolBuilder.setMaxSets(builder.descriptorSetCount);

		std::unordered_map<VkDescriptorType, uint32_t> typeToCount;
		for (auto &d : builder.descriptors){
			typeToCount[d.type] += d.type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ? d.imageCount : 1;
		}

		for (auto &t : typeToCount){
			poolBuilder.addPoolSize(t.first, t.second);
		}

		pool.initialize(poolBuilder);
	}

	void DescriptorSet::createSetLayout(DescriptorSetBuilder &builder){
		DescriptorSetLayoutBuilder layoutBuilder;

		for (auto &d : builder.descriptors){
			layoutBuilder.AddBinding(d.binding, d.type, d.stage, d.type == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ? d.imageCount : 1);
		}

		layout.initialize(layoutBuilder);
	}

	void DescriptorSet::createBuffers(DescriptorSetBuilder &builder){
		uint32_t offset = 0;
		for (uint32_t i=0; i<builder.descriptors.size(); i++){
			auto &d = builder.descriptors[i];

			if (d.type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER){
				buffers[i].size = d.bufferSize;
				buffers[i].alignement = getAlignment(d.bufferSize, getInstance().physicalDevice.getProperties().limits.minUniformBufferOffsetAlignment);

				buffers[i].offset = offset;

				offset += buffers[i].alignement;
			}
		}

		if (offset){
			buffer = new Buffer[descriptorSetCount];

			for (uint32_t i=0; i<descriptorSetCount; i++){
				buffer[i].alloc(offset, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
				buffer[i].map();
			}

			offset = 0;
			for (uint32_t i=0; i<builder.descriptors.size(); i++){
				auto &d = builder.descriptors[i];

				if (d.type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER){
					VkDescriptorBufferInfo info = {};
					info.offset = offset;
					info.range = d.bufferSize;
					
					d.bufferInfo = info;
					offset += d.bufferSize;
				} 
			}
		}
	}

	void DescriptorSet::createDescriptorSets(DescriptorSetBuilder &builder){

		for (uint32_t i=0; i<descriptorSetCount; i++){
			DescriptorWriter writer(layout, pool);
			
			for (auto &d : builder.descriptors){

				switch (d.type){
					case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:{
						d.bufferInfo.buffer = buffer[i].getBuffer();
						writer.writeBuffer(d.binding, &d.bufferInfo); 
						break;
					}
					case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: writer.writeImages(d.binding, d.imageCount, d.imageInfos.data()); break;
					default: throw std::runtime_error("invalid descriptor type"); break;
				}
			}

			writer.build(sets[i]);
		}
	}

	DescriptorSetLayout& DescriptorSet::getLayout(){
		return layout;
	}

	DescriptorPool& DescriptorSet::getPool(){
		return pool;
	}

	void* DescriptorSet::getBuffer(uint32_t setIndex, uint32_t binding){
		auto range = buffers[binding];
		auto &b = buffer[setIndex];
		char *ptr = reinterpret_cast<char*>(b.getMappedMemory());
		ptr += range.offset;
		return ptr;
	}

	void DescriptorSet::writeBuffer(uint32_t setIndex, uint32_t binding, void* data){
		auto range = buffers[binding];
		auto &b = buffer[setIndex];
		char *ptr = reinterpret_cast<char*>(b.getMappedMemory());
		ptr += range.offset;
		memcpy(ptr, data, range.size);
		b.flush(range.alignement, range.offset);
	}

	VkDescriptorSet* DescriptorSet::getSets(){
		return sets;
	}

	VkDescriptorSet DescriptorSet::getSet(uint32_t index){
		return sets[index];
	}
}