#include "engine/renderer/ShaderLibrary.hpp"

namespace engine{
	void ShaderLibrary::push(const Ref<Shader> &shader){
		shaders[shader->getName()] = shader;
	}

	void ShaderLibrary::push(const std::string &filepath){
		Ref<Shader> shader = Shader::create(filepath);
		shaders[shader->getName()] = shader;
	}

	void ShaderLibrary::push(const std::string &name, const std::string &filepath){
		Ref<Shader> shader = Shader::create(name, filepath);
		shaders[shader->getName()] = shader;
	}

	void ShaderLibrary::pop(const std::string &name){
		shaders.erase(name);
	}

	const Ref<Shader>& ShaderLibrary::get(const std::string &name){
		return shaders[name];
	}
}