#pragma once

#include "../core.hpp"
#include "../renderer/IndexBuffer.hpp"

namespace engine::OpenGl{
	class ENGINE_API OpenGlIndexBuffer : public IndexBuffer{
		public:
			OpenGlIndexBuffer(uint32_t *indices, uint32_t count);
			~OpenGlIndexBuffer();

			virtual void bind() const override;
			virtual void unBind() const override;
			virtual uint32_t getCount() const override {return count;}
		
		private:
			uint32_t buffer;
			uint32_t count;
	};
}