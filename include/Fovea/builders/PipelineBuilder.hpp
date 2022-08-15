#pragma once

#include "../../vulkan/vulkan.h"

#include <vector>
#include <array>
#include <filesystem>

namespace Fovea{
	struct PipelineConfigInfo{
		PipelineConfigInfo(){defaultPipelineConfigInfo(*this);}
		static void defaultPipelineConfigInfo(PipelineConfigInfo &configInfo);

		VkPipelineViewportStateCreateInfo viewportInfo{};
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
		VkPipelineMultisampleStateCreateInfo multisampleInfo{};
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
		std::vector<VkDynamicState> dynamicStateEnables{};
		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
		VkRenderPass renderPass = VK_NULL_HANDLE;
		uint32_t subpass = 0;
	};

	enum PipelineStage{
		VERTEX,
		GEOMETRY,
		FRAGMENT,
		COMPUTE,
		
		STAGE_COUNT
	};

	enum AttributFormat{
		FLOAT = VK_FORMAT_R32_SFLOAT,
		FLOAT_VEC2 = VK_FORMAT_R32G32_SFLOAT,
		FLOAT_VEC3 = VK_FORMAT_R32G32B32_SFLOAT,
		FLOAT_VEC4 = VK_FORMAT_R32G32B32A32_SFLOAT,
		INT = VK_FORMAT_R32_SINT,
		INT_VEC2 = VK_FORMAT_R32G32_SINT,
		INT_VEC3 = VK_FORMAT_R32G32B32_SINT,
		INT_VEC4 = VK_FORMAT_R32G32B32A32_SINT,
		UNSGINED_INT = VK_FORMAT_R32_UINT,
		UNSGINED_INT_VEC2 = VK_FORMAT_R32G32_UINT,
		UNSGINED_INT_VEC3 = VK_FORMAT_R32G32B32_UINT,
		UNSGINED_INT_VEC4 = VK_FORMAT_R32G32B32A32_UINT,
	};

	struct PipelineVertexDescription{
		VkVertexInputBindingDescription bindingDescription;
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
	};

	class Pipeline;
	class DescriptorSetLayout;

	class PipelineBuilder{
		friend class Pipeline;
		public:
			PipelineBuilder();
			PipelineConfigInfo* operator->() {return &config;}

			void setShaderStage(PipelineStage stage, const std::filesystem::path &path);

			template<typename T>
			void setPushConstant(int stages = PipelineStage::VERTEX | PipelineStage::FRAGMENT){
				setPushConstant(sizeof(T), stages);
			}
			
			void setPushConstant(size_t size, int stages);

			void setRenderPass(VkRenderPass renderPass);
			void setSubpass(uint32_t subpass);

			void setBase(Pipeline* base);

			void pushSetLayout(DescriptorSetLayout* layout);


			void setVertexDescription(const PipelineVertexDescription &description);
			
		private:
			struct ShaderStage{
				bool required = false;
				std::filesystem::path path;
				PipelineStage stage;
			};

			struct PushConstant{
				std::size_t size;
				int stages;
				std::uint8_t binding = 0;
			};

			std::array<ShaderStage, static_cast<size_t>(PipelineStage::STAGE_COUNT)> shaderStages;

			PipelineConfigInfo config;
			PushConstant pushConstant;
			Pipeline* base = nullptr;
			std::vector<DescriptorSetLayout*> setLayouts{};

			bool hasVertexDescription = false;
			PipelineVertexDescription vertexDescription;
	};
}