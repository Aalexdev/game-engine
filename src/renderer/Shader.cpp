#include "engine/pch.hpp"
#include "engine/renderer/Shader.hpp"
#include "engine/renderer/Renderer.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"

#include "engine/OpenGL_AL/OpenGL_Shader.hpp"

namespace engine{
	Ref<Shader> Shader::create(Renderer *renderer, const std::filesystem::path &filepath){
		ENGINE_PROFILE_FUNCTION();

		switch (renderer->getRenderAPI()){
			case RenderAPI::OPENGL: return OpenGL::Shader::create(renderer, filepath);
		}

		ENGINE_CORE_ASSERT(false, "unknown render API");
		return nullptr;
	}

	Ref<Shader> Shader::create(Renderer *renderer, const std::string &name, const std::filesystem::path &filepath){
		ENGINE_PROFILE_FUNCTION();

		switch (renderer->getRenderAPI()){
			case RenderAPI::OPENGL: return OpenGL::Shader::create(renderer, name, filepath);
		}

		ENGINE_CORE_ASSERT(false, "unknown render API");
		return nullptr;
	}
}