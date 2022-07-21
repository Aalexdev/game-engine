#include "horreum/SharedStackAllocator.hpp"
#include "horreum/Horreum.hpp"

SharedStackAllocator::SharedStackAllocator(size_t size) : maxSize(size){
	data = HRM_MALLOC(size);
}

SharedStackAllocator::~SharedStackAllocator(){
	HRM_FREE(data);
}

void* SharedStackAllocator::pushLeft(size_t size){
	void *ptr = static_cast<char*>(data) + sizeUsedLeft;
	setCurrentUsedSizeLeft(sizeUsedLeft + size);
	return ptr;
}

void* SharedStackAllocator::pushRight(size_t size){
	void *ptr = static_cast<char*>(data) + maxSize - sizeUsedRight - size;
	setCurrentUsedSizeRight(sizeUsedRight + size);
	return ptr;
}

size_t SharedStackAllocator::getCurrentUsedSizeLeft() const{
	return sizeUsedLeft;
}

size_t SharedStackAllocator::getCurrentUsedSizeRight() const{
	return sizeUsedRight;
}

void SharedStackAllocator::setCurrentUsedSizeLeft(size_t size){
	if (size > maxSize - sizeUsedRight){
		throw std::overflow_error("out of memory"); // TODO : change to the error handler
	}
	sizeUsedLeft = size;
}

void SharedStackAllocator::setCurrentUsedSizeRight(size_t size){
	if (size > maxSize - sizeUsedLeft){
		throw std::overflow_error("out of memory"); // TODO : change to the error handler
	}
	sizeUsedRight = size;
}

size_t SharedStackAllocator::getTotalUsedSize() const{
	return sizeUsedLeft + sizeUsedRight;
}

size_t SharedStackAllocator::getMaxSize() const{
	return maxSize;
}