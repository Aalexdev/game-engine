#pragma once

#include "../core.hpp"
#include "../renderer/Shader.hpp"

#include <libs/glad/glad.h>

namespace engine::OpenGl{
	class ENGINE_API OpenGlShader : public Shader{
		public:
			OpenGlShader(const std::string &name, const std::string &filepath);
            OpenGlShader(const std::string &filepath);
            ~OpenGlShader();

            virtual void bind() const override;
            virtual void unBind() const override;
            virtual inline const std::string& getName() const override {return name;}
            virtual inline void setName(const std::string &name) override {this->name = name;}

			void uploadUniformFloat(float value, const std::string &name) const;
			void uploadUniformFloat2(const glm::vec2 &value, const std::string &name) const;
			void uploadUniformFloat3(const glm::vec3 &value, const std::string &name) const;
			void uploadUniformFloat4(const glm::vec4 &value, const std::string &name) const;
			void uploadUniformInt(int value, const std::string &name) const;
			void uploadUniformInt2(const glm::ivec2 &value, const std::string &name) const;
			void uploadUniformInt3(const glm::ivec3 &value, const std::string &name) const;
			void uploadUniformInt4(const glm::ivec4 &value, const std::string &name) const;
			void uploadUniformMat2(const glm::mat2 &matrix, const std::string &name) const;
			void uploadUniformMat3(const glm::mat3 &matrix, const std::string &name) const;
			void uploadUniformMat4(const glm::mat4 &matrix, const std::string &name) const;
			void uploadUniformIntArray(int *values, uint32_t count, const std::string &name) const;

			virtual inline void setIntArray(int *values, uint32_t count, const std::string &name) override {uploadUniformIntArray(values, count, name);}
			virtual inline void setInt(int value, const std::string &name) override {uploadUniformInt(value, name);}
			virtual inline void setInt2(const glm::ivec2 &value, const std::string &name) override {uploadUniformInt2(value, name);}
			virtual inline void setInt3(const glm::ivec3 &value, const std::string &name) override {uploadUniformInt3(value, name);}
			virtual inline void setInt4(const glm::ivec4 &value, const std::string &name) override {uploadUniformInt4(value, name);}
			virtual inline void setFloat(float value, const std::string &name) override {uploadUniformFloat(value, name);}
			virtual inline void setFloat2(const glm::vec2& value, const std::string &name) override {uploadUniformFloat2(value, name);}
			virtual inline void setFloat3(const glm::vec3& value, const std::string &name) override {uploadUniformFloat3(value, name);}
			virtual inline void setFloat4(const glm::vec4& value, const std::string &name) override {uploadUniformFloat4(value, name);}
			virtual inline void setMat3(const glm::mat3 &value, const std::string &name) override {uploadUniformMat3(value, name);}
			virtual inline void setMat4(const glm::mat4 &value, const std::string &name) override {uploadUniformMat4(value, name);}

		private:
			static std::string readFile(const std::string &filepath);
			static GLenum ShaderTypeFromString(const std::string &type);
			static std::unordered_map<GLenum, std::string> preProcess(const std::string &shaderSource);
			void compile(std::unordered_map<GLenum, std::string> &sources);

			uint32_t shaderProgram;
			std::string name;
	};
}