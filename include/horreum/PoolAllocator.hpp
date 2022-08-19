#pragma once

#include <iostream>
#include <queue>
#include <list>

class PoolAllocator{
	public:
		PoolAllocator(size_t elementSize, size_t elementCount);
		~PoolAllocator();

		void* getElement();
		void restoreElement(void* element);

		size_t getRemainingElementCount();
		size_t getElementSize();
		size_t getMaxElementSize();
		size_t getUsedElementsCount();

	private:
		std::queue<void*> unusedElements;
		std::list<void*> elements;
		size_t maxElementCount;
		size_t elementUsedCount = 0;
		size_t elementSize;
};