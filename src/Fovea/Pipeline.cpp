#include "Fovea/Pipeline.hpp"
#include "Fovea/core.hpp"

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
	
		for (auto m : shaderModule){
			vkDestroyShaderModule(device, m.shaderModule, nullptr);
		}

		vkDestroyPipeline(device, pipeline, nullptr);
		vkDestroyPipelineLayout(device, layout, nullptr);
	}

	void Pipeline::initialize(PipelineBuilder *builder){

		createShaderModules(*builder);
		createPipelineLayout(*builder);
		createGraphicPipeline(*builder);
	}

	const PipelineConfigInfo& Pipeline::getConfig(){
		return config;
	}

	void Pipeline::bind(VkCommandBuffer commandBuffer){
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
	}

	void Pipeline::bindPushConstant(){
		
	}

	void Pipeline::createGraphicPipeline(PipelineBuilder &builder){
		auto &config = builder.config;
		
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages(shaderModule.size());

		for (int i=0; i<static_cast<int>(shaderStages.size()); i++){
			auto &stage = shaderStages[i];
			stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stage.module = shaderModule[i].shaderModule;
			stage.pName = "main";
			stage.flags = 0;
			stage.pNext = nullptr;
			stage.stage = shaderModule[i].stage;
			stage.pSpecializationInfo = nullptr;
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};

		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr;

		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexBindingDescriptions = nullptr;

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

		createInfo.basePipelineHandle = VK_NULL_HANDLE;
		createInfo.basePipelineIndex = -1;

		VkResult result = vkCreateGraphicsPipelines(getInstance().logicalDevice.getDevice(), VK_NULL_HANDLE, 1, &createInfo, nullptr, &pipeline);
		if (result != VK_SUCCESS){
			throw std::runtime_error("failed to create graphic pipeline");
		}
	}

	VkShaderStageFlags stageToVkStages(int stages){
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
		VkPushConstantRange range = {};
		range.offset = 0;
		range.size = builder.pushConstant.size;
		range.stageFlags = stageToVkStages(builder.pushConstant.stages);

		VkPipelineLayoutCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

		if (range.size > 0){
			createInfo.pushConstantRangeCount = 1;
			createInfo.pPushConstantRanges = &range;
		} else {
			createInfo.pushConstantRangeCount = 0;
			createInfo.pPushConstantRanges = nullptr;
		}

		// TODO
		createInfo.setLayoutCount = 0;
		createInfo.pSetLayouts = nullptr;

		createInfo.flags = 0;

		if (vkCreatePipelineLayout(getInstance().logicalDevice.getDevice(), &createInfo, nullptr, &layout) != VK_SUCCESS){
			throw std::runtime_error("failed to create pipeline layout");
		}
	}

	VkShaderStageFlagBits pipelineStageToVkStage(PipelineStage stage){
		switch (stage){
			case PipelineStage::VERTEX: return VK_SHADER_STAGE_VERTEX_BIT;
			case PipelineStage::GEOMETRY: return VK_SHADER_STAGE_GEOMETRY_BIT;
			case PipelineStage::FRAGMENT: return VK_SHADER_STAGE_FRAGMENT_BIT;
			case PipelineStage::COMPUTE: return VK_SHADER_STAGE_COMPUTE_BIT;
		}
		return VK_SHADER_STAGE_VERTEX_BIT;
	}

	void Pipeline::createShaderModules(PipelineBuilder &builder){
		std::vector<PipelineBuilder::ShaderStage> shaderStages;

		for (int i=0; i<static_cast<int>(builder.shaderStages.size()); i++){
			if (!builder.shaderStages[i].required) continue;
			shaderStages.push_back(builder.shaderStages[i]);
		}

		shaderModule.resize(shaderStages.size());

		for (int i=0; i<static_cast<int>(shaderModule.size()); i++){
			std::vector<char> code = readFile(shaderStages[i].path.string());
			shaderModule[i].stage = pipelineStageToVkStage(shaderStages[i].stage);
			createShaderModule(code, shaderModule[i].shaderModule);
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