#include "PagingAllocator.h"

PagingAllocator* PagingAllocator::sharedAllocator = nullptr;

PagingAllocator::PagingAllocator(size_t maxMem, size_t frameSize) : IMemoryAllocator() {
	this->maxMem = maxMem;
	this->frameSize = frameSize;
	this->numFrames = maxMem / frameSize;
	for (size_t i = 0; i < numFrames; i++) {
		freeFrameList.push_back(i);
	}
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

void* PagingAllocator::allocate(size_t size) {

}

void PagingAllocator::deallocate(void* ptr) {

}