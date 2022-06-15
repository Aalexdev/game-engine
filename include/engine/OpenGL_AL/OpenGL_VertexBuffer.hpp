#pragma once

#include "../core.hpp"
#include "../renderer/VertexBuffer.hpp"
#include "../renderer/Renderer.hpp"

namespace engine::OpenGL{
	class ENGINE_API VertexBuffer : public engine::VertexBuffer{
		public:
			static Ref<VertexBuffer> create(Renderer *renderer, float *vertices, uint32_t size);
			static Ref<VertexBuffer> create(Renderer *renderer, uint32_t size);

			VertexBuffer(Renderer *renderer, float *vertices, uint32_t size);
			VertexBuffer(Renderer *renderer, uint32_t size);
			~VertexBuffer();

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