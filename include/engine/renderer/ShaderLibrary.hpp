#pragma once

#include "../core.hpp"
#include "../pch.hpp"
#include "Shader.hpp"

namespace engine{
	class ENGINE_API ShaderLibrary{
		public:
			ShaderLibrary() {}
			~ShaderLibrary() {}

			void push(const Ref<Shader> &shader);
			void push(const std::string &filepath);
			void push(const std::string &name, const std::string &filepath);

			void pop(const std::string &name);

			const Ref<Shader>& get(const std::string &name);

		private:
			std::unordered_map<std::string, Ref<Shader>> shaders;
	};
}