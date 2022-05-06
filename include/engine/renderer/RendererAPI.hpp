#pragma once

#include "../core.hpp"
#include "VertexArray.hpp"

#include <libs/glm/glm.hpp>

namespace engine{
	class RendererAPI{
		public:
			virtual ~RendererAPI() {}

			virtual void init() = 0;
			virtual void clear() = 0;
			virtual void setClearColor(const glm::vec4& clearColor) = 0;

			// if the index count is 0, it will automaticaly set to the vertexArray's indexCount
			virtual void drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;
			virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

			static Ref<RendererAPI> create();

	};
}