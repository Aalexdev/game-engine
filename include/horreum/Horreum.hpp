#pragma once

#ifdef HORREUM_ASSERTS
	#pragma message "Horreum assert enabled"
#endif

#ifdef HORREUM_PROFILE
	#pragma message "Horreum profiling enabled"
#endif

#ifdef HORREUM_ALLOC_LOG
	#pragma message "Horreum allocations log enabled"
#endif

#include <iostream>
#include <filesystem>
#include <fstream>
#include <unordered_map>

class Horreum{
	public:
		Horreum();
		~Horreum();

		static Horreum& getInstance();
		static void initialize(const std::filesystem::path &logFile = "allocationLog.txt");

		static void* malloc(size_t size);
		static void* realloc(void *src, size_t newSize);
		static void free(void *ptr);

		static void* mallocLog(size_t size, const char *file, const char *functionName, int line);
		static void* reallocLog(void *src, size_t newSize, const char *file, const char *functionName, int line);
		static void freeLog(void *ptr);

	private:

		#ifdef HORREUM_ALLOC_LOG
			struct AllocationData{
				bool error = false;
				std::string function;
				std::string file;
				int line;
				size_t size;
			};

			uint32_t allocationCount = 0;
			uint32_t freeCount = 0;
			std::unordered_map<void*, AllocationData> allocMap;
			std::ofstream allocLogFile;

			void closeFile();
			void writeData(AllocationData &data);
		#endif
};

#ifdef HORREUM_ALLOC_LOG
	#define HRM_MALLOC(size) ::Horreum::mallocLog(size, __FILE__, __func__, __LINE__)
	#define HRM_REALLOC(ptr, size) ::Horreum::reallocLog(ptr, size, __FILE__, __func__, __LINE__)
	#define HRM_FREE(ptr) ::Horreum::freeLog(ptr)
#else
	#define HRM_MALLOC(size) ::Horreum::malloc(size)
	#define HRM_REALLOC(ptr, size) ::Horreum::realloc(ptr, size)
	#define HRM_FREE(ptr) ::Horreum::free(ptr)
#endif