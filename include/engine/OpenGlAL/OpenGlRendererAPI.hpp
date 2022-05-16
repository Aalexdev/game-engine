#pragma once

#include "../core.hpp"
#include "../renderer/RendererAPI.hpp"

namespace engine::OpenGl{
	class ENGINE_API OpenGlRendererAPI : public RendererAPI{
		public:
			OpenGlRendererAPI(){}
			~OpenGlRendererAPI(){}

			virtual void init() override;
			virtual void clear() override;
			virtual void setClearColor(const glm::vec4& clearColor) override;

			virtual void setLineThickness(float thickness) override;
			virtual void drawLines(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
			virtual void drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
			virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
	};
}