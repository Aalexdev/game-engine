#pragma once

#include <iostream>

class StackAllocator{
	public:
		StackAllocator(size_t size);
		~StackAllocator();

		void clear();
		void* push(size_t size);
		size_t getCurrentUsedSize();
		size_t getMaxSize();
		void setCurrentUsedSize(size_t size);

	private:
		void* data;
		size_t maxSize = 0;
		size_t currentUsedSize = 0;
};