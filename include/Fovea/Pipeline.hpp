#pragma once

#include "builders/PipelineBuilder.hpp"

namespace Fovea{
	class Pipeline{
		public:
			Pipeline() = default;
			Pipeline(const Pipeline &) = default;

			Pipeline(PipelineBuilder *builder);
			~Pipeline();

			void initialize(PipelineBuilder *builder);
			const PipelineConfigInfo& getConfig();
			void bind(VkCommandBuffer commandBuffer);
			void bindPushConstant(); 

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

			PipelineConfigInfo config;

			std::vector<ShaderModule> shaderModule;
			void *pushConstantData = nullptr;

	};
}