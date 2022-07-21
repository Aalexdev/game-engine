#include "horreum/StackAllocator.hpp"
#include "horreum/Horreum.hpp"

StackAllocator::StackAllocator(size_t size) : maxSize(size){
	data = HRM_MALLOC(size);
}

StackAllocator::~StackAllocator(){
	HRM_FREE(data);
	data = nullptr;
}

void StackAllocator::clear(){
	currentUsedSize = 0;
}

void* StackAllocator::push(size_t size){
	if (currentUsedSize + size > maxSize){
		throw std::overflow_error("out of memory");
	}

	void* ptr = static_cast<char*>(data)+currentUsedSize;
	currentUsedSize += size;
	return ptr;
}

size_t StackAllocator::getCurrentUsedSize(){
	return currentUsedSize;
}

void StackAllocator::setCurrentUsedSize(size_t size){
	if (size > maxSize){
		size = maxSize;
	}
	currentUsedSize = size;
}

size_t StackAllocator::getMaxSize(){
	return maxSize;
}
