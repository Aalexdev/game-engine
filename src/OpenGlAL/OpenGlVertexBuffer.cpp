#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/OpenGlAL/OpenGlVertexBuffer.hpp"

#include <libs/glad/glad.h>

namespace engine::OpenGl{
	OpenGlVertexBuffer::OpenGlVertexBuffer(float *vertices, uint32_t size){
        ENGINE_PROFILE_FUNCTION();
        glCreateBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	}

	OpenGlVertexBuffer::OpenGlVertexBuffer(uint32_t size){
        ENGINE_PROFILE_FUNCTION();
        glCreateBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);

	}

	OpenGlVertexBuffer::~OpenGlVertexBuffer(){
		ENGINE_PROFILE_FUNCTION();
		glDeleteBuffers(1, &buffer);
	}

	void OpenGlVertexBuffer::bind() const{
		ENGINE_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
	}

	void OpenGlVertexBuffer::unbind() const{
		ENGINE_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGlVertexBuffer::setLayout(const BufferLayout &layout){
		this->layout = layout;
	}
	
	const BufferLayout& OpenGlVertexBuffer::getLayout() const{
		return layout;
	}

	void OpenGlVertexBuffer::setData(const void *data, uint32_t size){
		ENGINE_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}
}