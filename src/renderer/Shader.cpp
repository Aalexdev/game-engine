#include "engine/pch.hpp"
#include "engine/renderer/Shader.hpp"
#include "engine/renderer/Renderer.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"

#include "engine/OpenGlAL/OpenGlShader.hpp"

namespace engine{
	Ref<Shader> Shader::create(const std::string &filepath){
		ENGINE_PROFILE_FUNCTION();

		switch (Renderer::getRenderAPI()){
			case Renderer::RenderAPI::OpenGL: return createRef<OpenGl::OpenGlShader>(filepath);
		}

		ENGINE_CORE_ASSERT(false, "unknown render API");
		return nullptr;
	}

	Ref<Shader> Shader::create(const std::string &name, const std::string &filepath){
		ENGINE_PROFILE_FUNCTION();

		switch (Renderer::getRenderAPI()){
			case Renderer::RenderAPI::OpenGL: return createRef<OpenGl::OpenGlShader>(name, filepath);
		}

		ENGINE_CORE_ASSERT(false, "unknown render API");
		return nullptr;
	}

}