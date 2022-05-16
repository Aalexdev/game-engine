#include "engine/pch.hpp"
#include "engine/debug/Log.hpp"
#include "engine/OpenGlAL/OpenGlIndexBuffer.hpp"
#include "engine/debug/Instrumentor.hpp"

#include <libs/glad/glad.h>

namespace engine::OpenGl{
	OpenGlIndexBuffer::OpenGlIndexBuffer(uint32_t *indices, uint32_t count){
		ENGINE_PROFILE_FUNCTION();
		glCreateBuffers(1, &buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * count, indices, GL_STATIC_DRAW);
	}

	OpenGlIndexBuffer::~OpenGlIndexBuffer(){
		ENGINE_PROFILE_FUNCTION();
		glDeleteBuffers(1, &buffer);
	}

	void OpenGlIndexBuffer::bind() const{
		ENGINE_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
	}

	void OpenGlIndexBuffer::unBind() const{
		ENGINE_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}