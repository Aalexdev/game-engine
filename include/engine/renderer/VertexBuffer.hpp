#pragma once

#include "../core.hpp"
#include "BufferLayout.hpp"

namespace engine{
	class ENGINE_API VertexBuffer{
		public:
			virtual ~VertexBuffer(){}

			virtual void bind() const = 0;
			virtual void unbind() const = 0;

			virtual void setLayout(const BufferLayout &layout) = 0;
			virtual const BufferLayout& getLayout() const = 0;
			virtual void setData(const void *data, uint32_t size) = 0;

			static Ref<VertexBuffer> create(float *vertices, uint32_t size);
			static Ref<VertexBuffer> create(uint32_t size);
	};
}