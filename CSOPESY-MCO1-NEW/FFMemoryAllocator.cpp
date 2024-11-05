#include "FFMemoryAllocator.h"

FFMemoryAllocator* FFMemoryAllocator::sharedAllocator = nullptr;

FFMemoryAllocator::FFMemoryAllocator(size_t maxMem, size_t frameSize) : IMemoryAllocator() {
	this->maxMem = maxMem;
	this->frameSize = frameSize;
	this->numFrames = maxMem / frameSize;

}

void FFMemoryAllocator::initialize(size_t maxMem, size_t frameSize) {
	sharedAllocator = new FFMemoryAllocator(maxMem, frameSize);
}

void FFMemoryAllocator::destroy() {
	delete sharedAllocator;
}

FFMemoryAllocator* FFMemoryAllocator::getInstance() {
	return sharedAllocator;
}

void* allocate(size_t size) {

}