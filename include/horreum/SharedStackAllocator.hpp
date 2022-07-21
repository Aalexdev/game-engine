#pragma once

#include <iostream>

class SharedStackAllocator{
	public:
		SharedStackAllocator(size_t size);
		~SharedStackAllocator();

		void* pushLeft(size_t size);
		void* pushRight(size_t size);
		 
		size_t getCurrentUsedSizeLeft() const;
		size_t getCurrentUsedSizeRight() const;

		void setCurrentUsedSizeLeft(size_t size);
		void setCurrentUsedSizeRight(size_t size);

		size_t getTotalUsedSize() const;
		size_t getMaxSize() const;
	
	private:
		void* data;

		size_t maxSize = 0;
		size_t sizeUsedLeft = 0;
		size_t sizeUsedRight = 0;
};