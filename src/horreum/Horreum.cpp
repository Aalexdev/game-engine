#include "horreum/Horreum.hpp"
#include <cassert>
#include <cstring>

#ifdef HORREUM_ASSERTS
	#define HORREUM_ASSERT(x) assert(x)
#else
	#define HORREUM_ASSERT(x)
#endif

Horreum::Horreum(){}

Horreum::~Horreum(){
	#ifdef HORREUM_ALLOC_LOG
		closeFile();
	#endif
}

Horreum& Horreum::getInstance(){
	static Horreum instance;
	return instance;
}

void Horreum::initialize(const std::filesystem::path &logFile){
	Horreum &instance = getInstance();

	#ifdef HORREUM_ALLOC_LOG
		instance.allocLogFile.open(logFile, std::ios::out);
		assert(instance.allocLogFile.is_open() && "failed to create / open file");
	#endif
}

void Horreum::writeData(AllocationData &data){
	if (data.error){
		allocLogFile << "----------------- ERROR ----------------\n";
	}

	allocLogFile << "file        : " << data.file << '\n';
	allocLogFile << "function    : " << data.function << '\n';
	allocLogFile << "line        : " << data.line << '\n';
	allocLogFile << "leaked size : " << data.size << "\n\n";
	allocLogFile << "----------------------------------------\n";

	if (data.error){
		allocLogFile << "----------------- ERROR ----------------\n";
	}

	allocLogFile << '\n';
}

void Horreum::closeFile(){
	int32_t difference = (int32_t)allocationCount - (int32_t)freeCount;

	allocLogFile << "----------------------------------------\n";
	allocLogFile << "---- close Horreum memory subsystem ----\n";
	allocLogFile << "----------------------------------------\n\n";

	allocLogFile << "total allocations : " << std::to_string(allocationCount) << '\n';
	allocLogFile << "total frees       : " << std::to_string(freeCount) << '\n';
	allocLogFile << "difference        : " << std::to_string(difference) << "\n\n";
	
	if (difference > 0){
		allocLogFile << "----------------------------------------\n";
		allocLogFile << "-------------- leaked memory -----------\n";
		allocLogFile << "----------------------------------------\n\n\n";

		uint32_t i=0;
		for (auto &d : allocMap){
			auto &data = d.second;
			allocLogFile << "n° " << std::to_string(i) << '\n';
			writeData(data);
			i++;
		}
	}

	allocLogFile.close();
}

void* Horreum::malloc(size_t size){
	#ifdef HORREUM_ALLOC_LOG // if the allocation fails, we would like to check inside the mallocLog function and print the error in the log file
		void *ptr = ::malloc(size);
		return ptr;
	#else
		void *ptr = ::malloc(size);
		assert(ptr != nullptr && "failed allocation");
		return ptr;
	#endif
}

void Horreum::free(void *ptr){
	::free(ptr);
}

void* Horreum::realloc(void *src, size_t newSize){
	#ifdef HORREUM_ALLOC_LOG // if the reallocation fails, we would like to check inside the reallocLog function and print the error in the log file
		void *ptr = ::realloc(src, newSize);
		return ptr;
	#else
		void *ptr = ::realloc(src, newSize);
		assert(ptr != nullptr && "failed allocation");
		return ptr;
	#endif
}

#define COPY_DATA() data.file = file; data.function = functionName; data.line = line; data.size = size;

void* Horreum::mallocLog(size_t size, const char *file, const char *functionName, int line){
	AllocationData data;
	COPY_DATA()
	Horreum &instance = getInstance();
	instance.allocLogFile << "alloc : " << std::to_string(size) << " bytes\n";
	instance.allocLogFile.flush();

	void* ptr = malloc(size);
	data.error = ptr == nullptr;
	if (data.error){
		instance.writeData(data);
		instance.closeFile();
		exit(1);
	}

	instance.allocationCount++;
	instance.allocMap[ptr] = data;
	return ptr;
}

void* Horreum::reallocLog(void *src, size_t newSize, const char *file, const char *functionName, int line){
	Horreum &instance = getInstance();
	instance.allocLogFile << "realloc : " << std::to_string(newSize) << " bytes\n";
	instance.allocLogFile.flush();

	void* ptr = realloc(src, newSize);
	if (ptr == nullptr){
		instance.closeFile();
		exit(1);
	}

	return ptr;
}

void Horreum::freeLog(void *ptr){
	Horreum &instance = getInstance();
	instance.allocLogFile << "free\n";
	instance.allocLogFile.flush();
	instance.allocMap.erase(ptr);
	instance.freeCount++;
	free(ptr);
}