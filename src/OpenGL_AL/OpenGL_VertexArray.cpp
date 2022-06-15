#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/OpenGL_AL/OpenGL_VertexArray.hpp"

#include <libs/glad/glad.h>

namespace engine::OpenGL{
	static uint32_t getOpenGLType(DataType type){
		switch (type){
			case DataType::None    : return GL_NONE;
			case DataType::Float   : return GL_FLOAT;
			case DataType::Float2  : return GL_FLOAT;
			case DataType::Float3  : return GL_FLOAT;
			case DataType::Float4  : return GL_FLOAT;
			case DataType::Mat2    : return GL_FLOAT;
			case DataType::Mat3    : return GL_FLOAT;
			case DataType::Mat4    : return GL_FLOAT;
			case DataType::Int     : return GL_INT;
			case DataType::Int2    : return GL_INT;
			case DataType::Int3    : return GL_INT;
			case DataType::Int4    : return GL_INT;
			case DataType::Bool    : return GL_BOOL;
			case DataType::UInt    : return GL_UNSIGNED_INT;
			case DataType::UInt2   : return GL_UNSIGNED_INT;
			case DataType::UInt3   : return GL_UNSIGNED_INT;
			case DataType::UInt4   : return GL_UNSIGNED_INT;
		}
		ENGINE_CORE_ASSERT(false, "unknwon shaderDataType");
		return 0;
	}

	static bool isIntType(DataType type){
		switch (type){
			case DataType::Int     : return true;
			case DataType::Int2    : return true;
			case DataType::Int3    : return true;
			case DataType::Int4    : return true;
			case DataType::UInt    : return true;
			case DataType::UInt2   : return true;
			case DataType::UInt3   : return true;
			case DataType::UInt4   : return true;
		}
		return false;
	}

	Ref<VertexArray> VertexArray::create(Renderer *renderer){
		return createRef<VertexArray>(renderer);
	}

	VertexArray::VertexArray(Renderer *renderer){
		ENGINE_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &vertexArray);
	}

	VertexArray::~VertexArray(){
		ENGINE_PROFILE_FUNCTION();
		glDeleteVertexArrays(1, &vertexArray);
	}

	void VertexArray::bind() const{
		ENGINE_PROFILE_FUNCTION();
		glBindVertexArray(vertexArray);
	}

	void VertexArray::unBind() const{
		ENGINE_PROFILE_FUNCTION();
		glBindVertexArray(0);
	}

	void VertexArray::addVertexBuffer(const Ref<VertexBuffer> &buffer){
		ENGINE_PROFILE_FUNCTION();
		glBindVertexArray(vertexArray);
		buffer->bind();

		ENGINE_CORE_ASSERT(buffer->getLayout().getElements().size(), "cannot add a vertex buffer with a non initialized layout to a vertex array");

		auto &layout = buffer->getLayout();
		uint32_t index = 0;
		for (auto &element : layout){
			glEnableVertexAttribArray(index);
			if (isIntType(element.type)){
				glVertexAttribIPointer(index, element.getComponentCount(), getOpenGLType(element.type), layout.getStride(), reinterpret_cast<const void*>(element.offset));
			} else {
				glVertexAttribPointer(index, element.getComponentCount(), getOpenGLType(element.type), element.normalized, layout.getStride(), reinterpret_cast<const void*>(element.offset));
			}
			index++;
		}

		vertexBuffers.push_back(buffer);
	}

	void VertexArray::setIndexBuffer(const Ref<IndexBuffer> &buffer){
		ENGINE_PROFILE_FUNCTION();
		glBindVertexArray(vertexArray);
		buffer->bind();
		indexBuffer = buffer;
	}
}