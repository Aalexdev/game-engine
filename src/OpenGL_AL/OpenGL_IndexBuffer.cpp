#include "engine/pch.hpp"
#include "engine/debug/Log.hpp"
#include "engine/OpenGL_AL/OpenGL_IndexBuffer.hpp"
#include "engine/debug/Instrumentor.hpp"

#include <libs/glad/glad.h>

namespace engine::OpenGL{
	Ref<IndexBuffer> IndexBuffer::create(Renderer *renderer, uint32_t *indices, uint32_t count){
		return createRef<IndexBuffer>(renderer, indices, count);
	}

	IndexBuffer::IndexBuffer(Renderer *renderer, uint32_t *indices, uint32_t count){
		ENGINE_PROFILE_FUNCTION();
		glCreateBuffers(1, &buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);
	}

	IndexBuffer::~IndexBuffer(){
		ENGINE_PROFILE_FUNCTION();
		glDeleteBuffers(1, &buffer);
	}

	void IndexBuffer::bind() const{
		ENGINE_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	}

	void IndexBuffer::unBind() const{
		ENGINE_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}