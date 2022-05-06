#pragma once

#include "../pch.hpp"
#include "../core.hpp"

#include "IndexBuffer.hpp"
#include "VertexBuffer.hpp"

namespace engine{
	class ENGINE_API VertexArray{
		public:
			virtual ~VertexArray() {}

			virtual void bind() const = 0;
			virtual void unBind() const = 0;

			virtual void addVertexBuffer(const Ref<VertexBuffer> &buffer) = 0;
			virtual void setIndexBuffer(const Ref<IndexBuffer> &buffer) = 0;

			virtual const std::vector<Ref<VertexBuffer>> &getVertexBuffers() const = 0;
			virtual const Ref<IndexBuffer> &getIndexBuffer() const = 0;
			
			static Ref<VertexArray> create();
	};
}