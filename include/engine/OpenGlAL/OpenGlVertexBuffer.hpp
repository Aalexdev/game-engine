#pragma once

#include "../core.hpp"
#include "../renderer/VertexBuffer.hpp"

namespace engine::OpenGl{
	class ENGINE_API OpenGlVertexBuffer : public VertexBuffer{
		public:
			OpenGlVertexBuffer(float *vertices, uint32_t size);
			OpenGlVertexBuffer(uint32_t size);
			~OpenGlVertexBuffer();

			virtual void bind() const override;
			virtual void unbind() const override;

			virtual void setLayout(const BufferLayout &layout) override;
			virtual const BufferLayout& getLayout() const override;
			virtual void setData(const void *data, uint32_t size) override;
		
		private:
			uint32_t buffer;
			BufferLayout layout;
	};
}