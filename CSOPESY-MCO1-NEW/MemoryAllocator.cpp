#include "MemoryAllocator.h"

MemoryAllocator* MemoryAllocator::sharedAllocator = nullptr;
std::shared_mutex MemoryAllocator::memoryMutex;

MemoryAllocator::MemoryAllocator(size_t maxMem, size_t frameSize){
	this->maxMem = maxMem; //since maxMem is 16384, it's 16KB
	this->frameSize = frameSize;
	this->numFrames = maxMem / frameSize;
	this->visualizeCounter = 0;
}

void MemoryAllocator::initialize(size_t maxMem, size_t frameSize) {
	sharedAllocator = new MemoryAllocator(maxMem, frameSize);
}

void MemoryAllocator::initializeMemory() {
	if (numFrames <= 1) {
		this->allocator = MemoryAllocator::ALLOCATOR_TYPE::FLAT;
		for (int i = 0; i < this->maxMem; i++) { //one element per byte
			int x = -1; //if -1, that means its free
			this->flatMemory.push_back(x);
		}
		this->freeList.push_back(std::make_tuple(0,this->maxMem-1)); //the first contiguous memory is just the whole sapce, so from 0 to the maxMemory
	}
	else {
		this->allocator = MemoryAllocator::ALLOCATOR_TYPE::PAGING;
		for (int i = 0; i < this->numFrames; i++) {
			int frame = frameSize;
			this->freeFrameList.push_back(frameSize);
			this->frameMap[i] = -1;
		}
		this->numFreeFrames = this->numFrames;
	}
}

void MemoryAllocator::destroy() {
	delete sharedAllocator;
}

MemoryAllocator* MemoryAllocator::getInstance() {
	return sharedAllocator;
}

//-1 for false;
//use numFrames == -1 for FlatAllocator
int MemoryAllocator::IsMemoryAvailable(size_t size, int numFrames) {
	if (this->allocator == MemoryAllocator::ALLOCATOR_TYPE::FLAT) {
		for (size_t i = 0; i < this->freeList.size(); i++) {
			std::tuple<int, int> freeMem = this->freeList[i];
			if (std::get<1>(freeMem) - std::get<0>(freeMem) >= size) {
				return std::get<0>(freeMem);
			}
		}
	}
	else {
		if (this->numFreeFrames < numFrames) return 0;
	}
	return -1;
}

boolean MemoryAllocator::IsProcessInMemory(int pid) {
	if (this->allocator == MemoryAllocator::ALLOCATOR_TYPE::FLAT) {
		for (auto t : this->occupiedMemory) {
			if (std::get<0>(t) == pid) return true;
		}
	}
	else {
		for (auto x : this->frameMap) {
			if (x.second == pid) return true;
		}
	}
	return false;
}

void MemoryAllocator::allocate(size_t size, int pid) {
	std::unique_lock<std::shared_mutex> lock(memoryMutex);
	if (this->allocator == MemoryAllocator::ALLOCATOR_TYPE::FLAT) {
		//check if theres memory available
		size_t index = this->IsMemoryAvailable(size, -1);
		//if there is, remove that memory from the freeList, set it in flatMemory, and add it to occupiedMemory
		if (index >= 0) {
			//look for it in the freeList, and just add the size to the starting index in the freeList
			//ex. if we add the first process, and it takes 200 bytes, the initial freeList of (0, maxMem-1) becomes
			//(200,maxMem-1) since 0-199 are now being used by the first process
			for (auto t : this->freeList) {
				if (std::get<0>(t) == (int)index) {
					std::get<0>(t) = (int)(index + size);
				}
			}
			//occupiedMemory, we just push back the pid, the starting address, and ending address
			//in the same example as above, we'd push_back (0, 0, 199)
			this->occupiedMemory.push_back(std::make_tuple(pid,index,index+size-1));
			//just sets the memory to the pid
			for (index; index < size; index++) {
				flatMemory[index] = pid;
			}
		}
	}
}

void MemoryAllocator::deallocate(int pid) {
	std::unique_lock<std::shared_mutex> lock(memoryMutex);
	if (this->allocator == MemoryAllocator::ALLOCATOR_TYPE::FLAT) {
		int startIndex = -1;
		int endIndex = -1;
		//find start and end index of occupied memory
		for (int i = 0; i < this->occupiedMemory.size(); i++) {
			if (std::get<0>(this->occupiedMemory[i])==pid) {
				startIndex = std::get<1>(this->occupiedMemory[i]);
				endIndex = std::get<2>(this->occupiedMemory[i]);
				this->occupiedMemory.erase(occupiedMemory.begin() + i);
			}
		}
		//set it all to -1 (free memory)
		for (int i = startIndex; i <= endIndex; i++) {
			if (flatMemory[i] == pid) flatMemory[i] = -1;
		}
		//add it back to free list, and merge the new flat memory
		this->freeList.push_back(std::make_tuple(startIndex, endIndex));
		this->mergeFlatMemory();
	}
	else {
		//paging allocator, not implemented yet
		//idea is to set the frameMap frames to -1, then add those to freeFrameList
	}
}

void MemoryAllocator::mergeFlatMemory() {
	//basically just changes the start to the end or vice versa type beat
	for (int i = 0; i < this->freeList.size(); i++) {
		for (int j = i+1; j < this->freeList.size(); j++) {
			int start1 = std::get<0>(freeList[i]),
				end1 = std::get<1>(freeList[i]);
			int start2 = std::get<0>(freeList[j]),
				end2 = std::get<1>(freeList[j]);
			if (end1 == start2-1) {
				std::get<1>(freeList[i]) = end2; //end1 = end2
				freeList.erase(freeList.begin() + j);
				j = i + 1;
			}
			else if (end2 == start1 - 1) {
				std::get<0>(freeList[i]) = start2;
				freeList.erase(freeList.begin() + j);
				j = i + 1;
			}
		}
	}
}
