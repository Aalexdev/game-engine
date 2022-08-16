#include "Fovea/Pipeline.hpp"
#include "Fovea/core.hpp"
#include "Fovea/DescriptorSetLayout.hpp"

#include <fstream>
#include <stdexcept>

namespace Fovea{

	std::vector<char> readFile(const std::string &filepath){
		std::ifstream file(filepath, std::ios::ate | std::ios::binary);

		if (!file.is_open())
			throw std::runtime_error("failed to open : " + filepath);
		
		size_t fileSize = file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	Pipeline::Pipeline(PipelineBuilder *builder){
		initialize(builder);
	}

	Pipeline::~Pipeline(){
		VkDevice device = getInstance().logicalDevice.getDevice();

		bool shared = (*refCount) > 1;
		
		if (!shared){
			for (auto m : shaderModules){
				vkDestroyShaderModule(device, m.shaderModule, nullptr);
			}
		}

		vkDestroyPipeline(device, pipeline, nullptr);

		if (!shared){
			vkDestroyPipelineLayout(device, layout, nullptr);
		}

		(*refCount)--;
	}

	void Pipeline::initialize(PipelineBuilder *builder){
		if (builder->base){
			refCount = builder->base->refCount;
			(*refCount)++;
		}

		createShaderModules(*builder);
		createPipelineLayout(*builder);
		createGraphicPipeline(*builder);
	}

	const PipelineConfigInfo& Pipeline::getConfig(){
		return config;
	}

	void Pipeline::bind(VkCommandBuffer commandBuffer, uint32_t setsIndex[]){
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
		std::vector<VkDescriptorSet> descriptorSets(sets.size());

		for (uint32_t i=0; i<sets.size(); i++){
			descriptorSets[i] = sets[i]->getSet(setsIndex[i]);
		}

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, 0, static_cast<uint32_t>(sets.size()), descriptorSets.data(), 0, nullptr);
	}

	void Pipeline::bindPushConstant(VkCommandBuffer commandBuffer, void *data){
		vkCmdPushConstants(commandBuffer, layout, pushConstantStages, 0, pushConstantSize, data);
	}

	void Pipeline::createGraphicPipeline(PipelineBuilder &builder){
		auto &config = builder.config;
		
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages(shaderModules.size());

		for (int i=0; i<static_cast<int>(shaderStages.size()); i++){
			auto &stage = shaderStages[i];
			stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stage.module = shaderModules[i].shaderModule;
			stage.pName = "main";
			stage.flags = 0;
			stage.pNext = nullptr;
			stage.stage = shaderModules[i].stage;
			stage.pSpecializationInfo = nullptr;
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};

		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		if (builder.hasVertexDescription){
			vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(builder.vertexDescription.attributeDescriptions.size());
			vertexInputInfo.pVertexAttributeDescriptions = builder.vertexDescription.attributeDescriptions.data();

			vertexInputInfo.vertexBindingDescriptionCount = 1;
			vertexInputInfo.pVertexBindingDescriptions = &builder.vertexDescription.bindingDescription;
		} else {
			vertexInputInfo.vertexAttributeDescriptionCount = 0;
			vertexInputInfo.pVertexAttributeDescriptions = nullptr;

			vertexInputInfo.vertexBindingDescriptionCount = 0;
			vertexInputInfo.pVertexBindingDescriptions = nullptr;
		}

		VkGraphicsPipelineCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
		createInfo.pStages = shaderStages.data();

		createInfo.pVertexInputState = &vertexInputInfo;
		createInfo.pInputAssemblyState = &config.inputAssemblyInfo;
		createInfo.pViewportState = &config.viewportInfo;
		createInfo.pRasterizationState = &config.rasterizationInfo;
		createInfo.pMultisampleState = &config.multisampleInfo;
		createInfo.pDepthStencilState = &config.depthStencilInfo;
		createInfo.pColorBlendState = &config.colorBlendInfo;
		createInfo.pDynamicState = &config.dynamicStateInfo;

		createInfo.layout = layout;
		createInfo.renderPass = config.renderPass;
		createInfo.subpass = 0;

		if (builder.base){
			createInfo.basePipelineHandle = builder.base->pipeline;
		} else {
			createInfo.basePipelineHandle = VK_NULL_HANDLE;
		}

		createInfo.basePipelineIndex = -1;

		VkResult result = vkCreateGraphicsPipelines(getInstance().logicalDevice.getDevice(), VK_NULL_HANDLE, 1, &createInfo, nullptr, &pipeline);
		if (result != VK_SUCCESS){
			throw std::runtime_error("failed to create graphic pipeline");
		}
	}

	static inline VkShaderStageFlags stageToVkStages(int stages){
		VkShaderStageFlags vkStages = 0;

		if (stages & PipelineStage::VERTEX){
			vkStages |= VK_SHADER_STAGE_VERTEX_BIT;
		}

		if (stages & PipelineStage::GEOMETRY){
			vkStages |= VK_SHADER_STAGE_GEOMETRY_BIT;
		}

		if (stages & PipelineStage::FRAGMENT){
			vkStages |= VK_SHADER_STAGE_FRAGMENT_BIT;
		}

		if (stages & PipelineStage::COMPUTE){
			vkStages |= VK_SHADER_STAGE_COMPUTE_BIT;
		}

		return stages;
	}

	void Pipeline::createPipelineLayout(PipelineBuilder &builder){
		if (builder.base){
			layout = builder.base->layout;
			return;
		}

		VkPushConstantRange range = {};
		range.offset = 0;
		range.size = builder.pushConstant.size;
		range.stageFlags = stageToVkStages(builder.pushConstant.stages);

		pushConstantSize = builder.pushConstant.size;
		pushConstantStages = range.stageFlags;

		VkPipelineLayoutCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		if (range.size > 0){
			createInfo.pushConstantRangeCount = 1;
			createInfo.pPushConstantRanges = &range;
		} else {
			createInfo.pushConstantRangeCount = 0;
			createInfo.pPushConstantRanges = nullptr;
		}

		std::vector<VkDescriptorSetLayout> layouts(builder.sets.size());
		for (int i=0; i<layouts.size(); i++){
			layouts[i] = builder.sets[i]->getLayout().getDescriptorSetLayout();
		}
		
		if (layouts.empty()){
			createInfo.setLayoutCount = 0;
			createInfo.pSetLayouts = nullptr;
		} else {
			createInfo.setLayoutCount = static_cast<uint32_t>(layouts.size());
			createInfo.pSetLayouts = layouts.data();

			sets.resize(builder.sets.size());
			for (size_t i=0; i<sets.size(); i++){
				sets[i] = builder.sets[i];
			}
		}

		createInfo.flags = 0;

		if (vkCreatePipelineLayout(getInstance().logicalDevice.getDevice(), &createInfo, nullptr, &layout) != VK_SUCCESS){
			throw std::runtime_error("failed to create pipeline layout");
		}
	}

	static inline VkShaderStageFlagBits pipelineStageToVkStage(PipelineStage stage){
		switch (stage){
			case PipelineStage::VERTEX: return VK_SHADER_STAGE_VERTEX_BIT;
			case PipelineStage::GEOMETRY: return VK_SHADER_STAGE_GEOMETRY_BIT;
			case PipelineStage::FRAGMENT: return VK_SHADER_STAGE_FRAGMENT_BIT;
			case PipelineStage::COMPUTE: return VK_SHADER_STAGE_COMPUTE_BIT;
		}
		return VK_SHADER_STAGE_VERTEX_BIT;
	}

	void Pipeline::createShaderModules(PipelineBuilder &builder){
		if (builder.base){
			shaderModules.resize(builder.base->shaderModules.size());
			shaderModules = builder.base->shaderModules;
			return;
		}

		std::vector<PipelineBuilder::ShaderStage> shaderStages;

		for (int i=0; i<static_cast<int>(builder.shaderStages.size()); i++){
			if (!builder.shaderStages[i].required) continue;
			shaderStages.push_back(builder.shaderStages[i]);
		}

		shaderModules.resize(shaderStages.size());

		for (int i=0; i<static_cast<int>(shaderModules.size()); i++){
			std::vector<char> code = readFile(shaderStages[i].path.string());
			shaderModules[i].stage = pipelineStageToVkStage(shaderStages[i].stage);
			createShaderModule(code, shaderModules[i].shaderModule);
		}
	}

	void Pipeline::createShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule){
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
		createInfo.codeSize = static_cast<uint32_t>(code.size());

		if (vkCreateShaderModule(getInstance().logicalDevice.getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS){
			throw std::runtime_error("failed to create shader module");
		}
	}
}