#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/OpenGL_AL/OpenGL_VertexBuffer.hpp"

#include <libs/glad/glad.h>

namespace engine::OpenGL{
	Ref<VertexBuffer> VertexBuffer::create(Renderer *renderer, float *vertices, uint32_t size){
		return createRef<VertexBuffer>(renderer, vertices, size);
	}

	Ref<VertexBuffer> VertexBuffer::create(Renderer *renderer, uint32_t size){
		return createRef<VertexBuffer>(renderer, size);
	}

	VertexBuffer::VertexBuffer(Renderer *renderer, float *vertices, uint32_t size){
        ENGINE_PROFILE_FUNCTION();
        glCreateBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	VertexBuffer::VertexBuffer(Renderer *renderer, uint32_t size){
        ENGINE_PROFILE_FUNCTION();
        glCreateBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer(){
		ENGINE_PROFILE_FUNCTION();
		glDeleteBuffers(1, &buffer);
	}

	void VertexBuffer::bind() const{
		ENGINE_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
	}

	void VertexBuffer::unbind() const{
		ENGINE_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::setLayout(const BufferLayout &layout){
        ENGINE_PROFILE_FUNCTION();
		this->layout = layout;
	}
	
	const BufferLayout& VertexBuffer::getLayout() const{
        ENGINE_PROFILE_FUNCTION();
		return layout;
	}

	void VertexBuffer::setData(const void *data, uint32_t size){
		ENGINE_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}
}