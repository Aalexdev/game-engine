#pragma once

#include "../pch.hpp"
#include "../core.hpp"

#include <libs/glm/glm.hpp>

namespace engine{
	class ENGINE_API Shader{
		public:
			virtual ~Shader() {}

			static Ref<Shader> create(const std::string &filepath);
			static Ref<Shader> create(const std::string &name, const std::string &filepath);

			virtual void bind() const = 0;
			virtual void unBind() const = 0;
			virtual const std::string &getName() const = 0;
			virtual void setName(const std::string &name) = 0;

			virtual void setIntArray(int *values, uint32_t count, const std::string &name) = 0;
			virtual void setInt(int value, const std::string &name) = 0;
			virtual void setInt2(const glm::ivec2 &value, const std::string &name) = 0;
			virtual void setInt3(const glm::ivec3 &value, const std::string &name) = 0;
			virtual void setInt4(const glm::ivec4 & value, const std::string &name) = 0;
			virtual void setFloat(float value, const std::string &name) = 0;
			virtual void setFloat2(const glm::vec2& value, const std::string &name) = 0;
			virtual void setFloat3(const glm::vec3& value, const std::string &name) = 0;
			virtual void setFloat4(const glm::vec4& value, const std::string &name) = 0;
			virtual void setMat3(const glm::mat3 &value, const std::string &name) = 0;
			virtual void setMat4(const glm::mat4 &value, const std::string &name) = 0;
		
		protected:
	};
}