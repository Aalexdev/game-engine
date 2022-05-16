#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/OpenGlAL/OpenGlRendererAPI.hpp"

#include <libs/glad/glad.h>

namespace engine::OpenGl{
	void OpenGlRendererAPI::init(){
		ENGINE_PROFILE_FUNCTION();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGlRendererAPI::clear(){
		ENGINE_PROFILE_FUNCTION();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGlRendererAPI::setClearColor(const glm::vec4& clearColor){
		ENGINE_PROFILE_FUNCTION();
		glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	}

	void OpenGlRendererAPI::drawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount){
		ENGINE_PROFILE_FUNCTION();
        uint32_t count = indexCount == 0 ? vertexArray->getIndexBuffer()->getCount() : indexCount;
		vertexArray->bind();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGlRendererAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height){
		ENGINE_PROFILE_FUNCTION();
        glViewport(x, y, width, height);
	}

	void OpenGlRendererAPI::drawLines(const Ref<VertexArray>& vertexArray, uint32_t indexCount){
		ENGINE_PROFILE_FUNCTION();
        // uint32_t count = indexCount == 0 ? vertexArray->getIndexBuffer()->getCount() : indexCount;
		vertexArray->bind();
		glDrawArrays(GL_LINES, 0, indexCount);
	}

	void OpenGlRendererAPI::setLineThickness(float thickness){
		ENGINE_PROFILE_FUNCTION();
		glLineWidth(thickness);
	};
}