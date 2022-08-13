#pragma once

#include "builders/PipelineBuilder.hpp"

namespace Fovea{
	class Pipeline{
		friend class PipelineLibrary;
		public:
			Pipeline() = default;
			Pipeline(const Pipeline &) = delete;

			Pipeline(PipelineBuilder *builder);
			~Pipeline();

			void initialize(PipelineBuilder *builder);
			const PipelineConfigInfo& getConfig();
			void bind(VkCommandBuffer commandBuffer);
			void bindPushConstant(VkCommandBuffer, void* data); 

		private:
			struct ShaderModule{
				VkShaderModule shaderModule;
				VkShaderStageFlagBits stage;
			};

			void createGraphicPipeline(PipelineBuilder &builder);
			void createPipelineLayout(PipelineBuilder &builder);
			void createShaderModules(PipelineBuilder &builder);
			void createShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule);

			VkPipeline pipeline = VK_NULL_HANDLE;
			VkPipelineLayout layout = VK_NULL_HANDLE;
			uint32_t pushConstantSize = 0;
			VkShaderStageFlags pushConstantStages;

			PipelineConfigInfo config;
			std::vector<ShaderModule> shaderModules;

			size_t *refCount = nullptr;
	};
}