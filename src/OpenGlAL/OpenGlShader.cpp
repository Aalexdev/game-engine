
#include "engine/pch.hpp"
#include "engine/debug/Log.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/OpenGlAL/OpenGlShader.hpp"

#include <libs/glad/glad.h>
#include <libs/glm/gtc/type_ptr.hpp>

namespace engine::OpenGl{
	OpenGlShader::OpenGlShader(const std::string &filepath){
		ENGINE_PROFILE_FUNCTION();

		std::string source = readFile(filepath);
		auto shaderSources = preProcess(source);
		compile(shaderSources);

		size_t lastSlash = filepath.find_last_of("/\\");

		if (lastSlash == std::string::npos) lastSlash = 0;
		else ++lastSlash;

		size_t lastDot = filepath.find_last_of(".");
		if (lastDot == std::string::npos) lastDot = filepath.size();

		name = filepath.substr(lastSlash, lastDot - lastSlash);
	}

	OpenGlShader::OpenGlShader(const std::string &name, const std::string &filepath){
		ENGINE_PROFILE_FUNCTION();
		this->name = name;
		
		std::string source = readFile(filepath);
		auto shaderSources = preProcess(source);
		compile(shaderSources);
	}

	GLenum OpenGlShader::ShaderTypeFromString(const std::string &type){
		if (type == "vertex") return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

		ENGINE_CORE_ASSERT(false, "unknown shader type : \"", type, "\"");
		return 0;
	}

	std::unordered_map<GLenum, std::string> OpenGlShader::preProcess(const std::string &shaderSource){
		ENGINE_PROFILE_FUNCTION();
		std::unordered_map<GLenum, std::string> sources;

		std::string typeKeyword = "#type";

		size_t pos = shaderSource.find(typeKeyword.c_str(), 0);
		while (pos != std::string::npos){
			size_t eol = shaderSource.find_first_of("\r\n", pos);
			ENGINE_CORE_ASSERT(eol != std::string::npos, "syntax error");
			size_t begin = pos + typeKeyword.size() + 1;
			std::string type = shaderSource.substr(begin, eol - begin);
			ENGINE_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "inavlid shader type");

			size_t nextLinePos = shaderSource.find_first_of("\r\n", eol);
			pos = shaderSource.find(typeKeyword, nextLinePos);
			sources[ShaderTypeFromString(type)] = shaderSource.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? shaderSource.size() - 1 : nextLinePos));
		}
		return sources;

	}

	std::string OpenGlShader::readFile(const std::string& filepath){
		ENGINE_PROFILE_FUNCTION();
		std::ifstream file(filepath, std::ios::in | std::ios::binary);

		if (!file.is_open()){
			ENGINE_CORE_ERROR("failed to open \"", filepath, "\"");
			return "";
		}
		
		std::string buffer;
		size_t size;

		file.seekg(0, std::ios::end);
		size = file.tellg();

		buffer.resize(size);
		file.seekg(0, std::ios::beg);
		file.read(&buffer[0], size);

		file.close();
		return buffer;
	}

	void OpenGlShader::compile(std::unordered_map<GLenum, std::string> &sources){
		ENGINE_PROFILE_FUNCTION();
		ENGINE_CORE_ASSERT(sources.size() <= 2, "only a maximum of two types of shaders are supported per shaders");
		std::array<uint32_t, 2> shaders;
		bool compiled = false;

		shaderProgram = glCreateProgram();

		int i=0;
		for (auto &source : sources){
			ENGINE_PROFILE_SCOPE("compile shader");

			shaders[i] = glCreateShader(source.first);
			const GLchar *shaderSource = (const GLchar *) source.second.c_str();
			glShaderSource(shaders[i], 1, &shaderSource, 0);

			glCompileShader(shaders[i]);

			GLint isCompiled = 0;
			glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &isCompiled);

			if(isCompiled == GL_FALSE){
				GLint maxLength = 0;
				glGetShaderiv(shaders[i], GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shaders[i], maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shaders[i]);
				ENGINE_CORE_ASSERT(false, "shaders[i] compilation failed : ", infoLog.data());
				break;
			}

			glAttachShader(shaderProgram, shaders[i]);
			++i;

			if (i == sources.size()) compiled = true;
		}
		
		if (!compiled){
			for (auto &shader : shaders){
				if (shader == 0) break; // is it the end of the compiled shaders
				glDetachShader(shaderProgram, shader);
				glDeleteShader(shader);
			}
			glDeleteProgram(shaderProgram);
			return;
		}

		{
			ENGINE_PROFILE_SCOPE("link shader");
			glLinkProgram(shaderProgram);
		}

		GLint isLinked = 0;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isLinked);
		if (isLinked == GL_FALSE){
			GLint maxLength = 0;
			glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);
			
			glDeleteProgram(shaderProgram);

			for (auto &shader : shaders){
				glDeleteShader(shader);
			}

			ENGINE_CORE_ASSERT(false, "failed to link shaders : ", infoLog.data());
			return;
		}

		{
			ENGINE_PROFILE_SCOPE("detach shaders");
			for (auto &shader : shaders){
				glDetachShader(shaderProgram, shader);
			}
		}
	}

	OpenGlShader::~OpenGlShader(){
		ENGINE_PROFILE_FUNCTION();
		glDeleteProgram(shaderProgram);
	}

	void OpenGlShader::bind() const{
		ENGINE_PROFILE_FUNCTION();
		glUseProgram(shaderProgram);
	}

	void OpenGlShader::unBind() const{
		ENGINE_PROFILE_FUNCTION();
		glUseProgram(0);
	}

	void OpenGlShader::uploadUniformFloat(float value, const std::string &name) const{
		int location = glGetUniformLocation(shaderProgram, name.c_str());
		ENGINE_CORE_ASSERT(location != -1, "cannot find \"", name, "\" uniform");
		glUniform1f(location, value);
	}

	void OpenGlShader::uploadUniformFloat2(const glm::vec2 &value, const std::string &name) const{
		int location = glGetUniformLocation(shaderProgram, name.c_str());
		ENGINE_CORE_ASSERT(location != -1, "cannot find \"", name, "\" uniform");
		glUniform2f(location, value.x, value.y);
	}

	void OpenGlShader::uploadUniformFloat3(const glm::vec3 &value, const std::string &name) const{
		int location = glGetUniformLocation(shaderProgram, name.c_str());
		ENGINE_CORE_ASSERT(location != -1, "cannot find \"", name, "\" uniform");
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGlShader::uploadUniformFloat4(const glm::vec4 &value, const std::string &name) const{
		int location = glGetUniformLocation(shaderProgram, name.c_str());
		ENGINE_CORE_ASSERT(location != -1, "cannot find \"", name, "\" uniform");
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGlShader::uploadUniformInt(int value, const std::string &name) const{
		int location = glGetUniformLocation(shaderProgram, name.c_str());
		ENGINE_CORE_ASSERT(location != -1, "cannot find \"", name, "\" uniform");
		glUniform1i(location, value);
	}

	void OpenGlShader::uploadUniformInt2(const glm::ivec2 &value, const std::string &name) const{
		int location = glGetUniformLocation(shaderProgram, name.c_str());
		ENGINE_CORE_ASSERT(location != -1, "cannot find \"", name, "\" uniform");
		glUniform2i(location, value.x, value.y);
	}

	void OpenGlShader::uploadUniformInt3(const glm::ivec3 &value, const std::string &name) const{
		int location = glGetUniformLocation(shaderProgram, name.c_str());
		ENGINE_CORE_ASSERT(location != -1, "cannot find \"", name, "\" uniform");
		glUniform3i(location, value.x, value.y, value.z);
	}

	void OpenGlShader::uploadUniformInt4(const glm::ivec4 &value, const std::string &name) const{
		int location = glGetUniformLocation(shaderProgram, name.c_str());
		ENGINE_CORE_ASSERT(location != -1, "cannot find \"", name, "\" uniform");
		glUniform4i(location, value.x, value.y, value.z, value.w);
	}

	void OpenGlShader::uploadUniformMat2(const glm::mat2 &matrix, const std::string &name) const{
		int location = glGetUniformLocation(shaderProgram, name.c_str());
		ENGINE_CORE_ASSERT(location != -1, "cannot find \"", name, "\" uniform");
		glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGlShader::uploadUniformMat3(const glm::mat3 &matrix, const std::string &name) const{
		int location = glGetUniformLocation(shaderProgram, name.c_str());
		ENGINE_CORE_ASSERT(location != -1, "cannot find \"", name, "\" uniform");
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGlShader::uploadUniformMat4(const glm::mat4 &matrix, const std::string &name) const{
		int location = glGetUniformLocation(shaderProgram, name.c_str());
		ENGINE_CORE_ASSERT(location != -1, "cannot find \"", name, "\" uniform");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGlShader::uploadUniformIntArray(int *values, uint32_t count, const std::string &name) const{
		int location = glGetUniformLocation(shaderProgram, name.c_str());
		ENGINE_CORE_ASSERT(location != -1, "cannot find \"", name, "\" uniform");
		glUniform1iv(location, count, values);
	}
}