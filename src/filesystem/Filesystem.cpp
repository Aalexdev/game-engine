#include "engine/pch.hpp"
#include "engine/debug/Instrumentor.hpp"
#include "engine/debug/Log.hpp"
#include "engine/filesystem/Filesystem.hpp"

namespace engine{
	std::filesystem::path Filesystem::executablePath;
	std::filesystem::path Filesystem::dataPath;
	std::string Filesystem::dataFolderName = ".data";

	
	void Filesystem::init(int argc, char **argv){
		ENGINE_CORE_ASSERT(argc > 0, "invalid arguments");
		executablePath = std::filesystem::path(argv[0]);

		std::filesystem::path executableFolder = executablePath.parent_path();

		for (auto &directory : std::filesystem::directory_iterator(executableFolder)){
			if (!directory.is_directory()) continue;
			if (directory.path().filename() == dataFolderName){
				dataPath = directory.path();
				break;
			}
		}

		if (dataPath.empty()){
			executableFolder = executableFolder.parent_path();
			for (auto &directory : std::filesystem::directory_iterator(executableFolder)){
			if (!directory.is_directory()) continue;
				if (directory.path().filename() == dataFolderName){
					dataPath = directory;
					break;
				}
			}
		}

		if (dataPath.empty())
			ENGINE_CORE_ERROR("cannot find data directory");
	}
}