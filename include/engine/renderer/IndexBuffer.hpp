#pragma once

#include "../core.hpp"

namespace engine{
	class ENGINE_API IndexBuffer{
		public:
			virtual ~IndexBuffer() {}

			virtual void bind() const = 0;
			virtual void unBind() const = 0;
			virtual uint32_t getCount() const = 0;
			
			static Ref<IndexBuffer> create(uint32_t* indices, uint32_t count);
	};
}