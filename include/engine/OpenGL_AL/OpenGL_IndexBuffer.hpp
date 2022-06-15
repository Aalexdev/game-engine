#pragma once

#include "../core.hpp"
#include "../renderer/IndexBuffer.hpp"
#include "../renderer/Renderer.hpp"

namespace engine::OpenGL{
	class ENGINE_API IndexBuffer : public engine::IndexBuffer{
		public:
			static Ref<IndexBuffer> create(Renderer *renderer, uint32_t *indices, uint32_t count);

			IndexBuffer(Renderer *renderer, uint32_t *indices, uint32_t count);
			~IndexBuffer();

			virtual void bind() const override;
			virtual void unBind() const override;
			virtual uint32_t getCount() const override {return count;}
		
		private:
			uint32_t buffer;
			uint32_t count;
	};
}