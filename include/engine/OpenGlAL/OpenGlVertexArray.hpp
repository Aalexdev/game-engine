#pragma once

#include "../core.hpp"
#include "../renderer/VertexArray.hpp"

namespace engine::OpenGl{
	class ENGINE_API OpenGlVertexArray : public VertexArray{
		public:
			OpenGlVertexArray();
			~OpenGlVertexArray();

			virtual void bind() const override;
			virtual void unBind() const override;

			virtual void addVertexBuffer(const Ref<VertexBuffer> &buffer) override;
			virtual void setIndexBuffer(const Ref<IndexBuffer> &buffer) override;

			virtual inline const std::vector<Ref<VertexBuffer>> &getVertexBuffers() const override {return vertexBuffers;}
			virtual inline const Ref<IndexBuffer> &getIndexBuffer() const override {return indexBuffer;}

		private:
			std::vector<Ref<VertexBuffer>> vertexBuffers;
			Ref<IndexBuffer> indexBuffer;
			uint32_t vertexArray;
	};
}