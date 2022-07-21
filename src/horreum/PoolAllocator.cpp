#include "horreum/PoolAllocator.hpp"
#include "horreum/Horreum.hpp"

PoolAllocator::PoolAllocator(size_t elementSize, size_t elementCount) : maxElementCount{elementCount}, elementSize{elementSize}{
	for (int i=0; i<elementCount; i++){
		void* element = HRM_MALLOC(elementSize);
		unusedElements.push(element);
		elements.push_back(element);
	}
}

PoolAllocator::~PoolAllocator(){
	for (auto &e : elements){
		HRM_FREE(e);
	}
}

void* PoolAllocator::getElement(){
	void* element = unusedElements.front();
	unusedElements.pop();
	return element;
}

void PoolAllocator::restoreElement(void* element){
	unusedElements.push(element);
}

size_t PoolAllocator::getRemainingElementCount(){
	return unusedElements.size();
}

size_t PoolAllocator::getElementSize(){
	return elementSize;
}

size_t PoolAllocator::getMaxElementSize(){
	return maxElementCount;
}

size_t PoolAllocator::getUsedElementsCount(){
	return maxElementCount - getRemainingElementCount();
}
