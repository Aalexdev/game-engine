#pragma once

#include "../core.hpp"
#include "../renderer/VertexArray.hpp"
#include "../renderer/Renderer.hpp"

namespace engine::OpenGL{
	class ENGINE_API VertexArray : public engine::VertexArray{
		public:
			static Ref<VertexArray> create(Renderer *renderer);
			VertexArray(Renderer *renderer);
			~VertexArray();

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