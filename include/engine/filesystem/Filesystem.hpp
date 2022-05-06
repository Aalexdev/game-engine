#pragma once

#include "../core.hpp"

namespace engine{
	class ENGINE_API Filesystem{
		public:
			static void init(int argc, char **argv);

			static inline const std::filesystem::path& getExecutablePath() {return executablePath;}
			static inline const std::filesystem::path& getDataFolderPath() {return dataPath;}

			static inline void setDataFolderName(const std::string &name) {dataFolderName = name;}
			static inline const std::string& getDataFolderName() {return dataFolderName;}
		
		private:
			static std::string dataFolderName;
			static std::filesystem::path executablePath;
			static std::filesystem::path dataPath;
	};
}