#include "FFMemoryAllocator.h"

PagingAllocator* PagingAllocator::sharedAllocator = nullptr;

PagingAllocator::PagingAllocator(size_t maxMem, size_t frameSize) : IMemoryAllocator() {
	this->maxMem = maxMem;
	this->frameSize = frameSize;
	this->numFrames = maxMem / frameSize;

}

void PagingAllocator::initialize(size_t maxMem, size_t frameSize) {
	sharedAllocator = new PagingAllocator(maxMem, frameSize);
}

void PagingAllocator::destroy() {
	delete sharedAllocator;
}

PagingAllocator* PagingAllocator::getInstance() {
	return sharedAllocator;
}

void* allocate(size_t size) {

}