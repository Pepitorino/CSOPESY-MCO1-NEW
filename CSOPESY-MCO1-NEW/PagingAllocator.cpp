#include "PagingAllocator.h"

PagingAllocator* PagingAllocator::sharedAllocator = nullptr;

std::shared_mutex PagingAllocator::memoryMutex;

PagingAllocator::PagingAllocator(size_t maxMem, size_t frameSize) : IMemoryAllocator() {
	this->maxMem = maxMem; //since maxMem is 16384, it's 16KB
	this->frameSize = frameSize;
	this->numFrames = maxMem / frameSize;
	this->visualizeCounter = 0;
	//initialize memory with -1 maxMem size
	this->memory = std::vector<int>(maxMem, -1);
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

//to be called by Scheduler
boolean PagingAllocator::IsMemoryAvailable(size_t size) {
	std::unique_lock<std::shared_mutex> lock(memoryMutex);
	for (int i = 0; i <= maxMem - size; i += 4096) {
		if (memory[i] == -1) {
			return true;
		}
	}
	return false;
}

//to be called by Scheduler and CPU, should return true if process is in memory, false if not
boolean PagingAllocator::IsProcessInMemory(int pid) {
	std::unique_lock<std::shared_mutex> lock(memoryMutex);
	for (int i = 0; i < occupiedFrames.size(); i++) {
		if (std::get<0>(occupiedFrames[i]) == pid) {
			return true;
		}
	}
}

//should 
void PagingAllocator::visualizeMemory2(int coreId, u_int qqCycle) {
	//selecting folder to save the memory visualization to be added
	std::unique_lock<std::shared_mutex> lock(memoryMutex);
	
	// named the file as 
}

//allocate2 should return true if successful, false if not
void PagingAllocator::allocate2(int pid, size_t size) {
	std::unique_lock<std::shared_mutex> lock(memoryMutex);
	//find the first free frame
	int Memindex = -1;
	for (int i = 0; i <= maxMem - size; i += 4096) {
		if (memory[i] == -1) {
			Memindex = i;
			break;
		}
	}
	//will never reach this point since memory is checked before calling this function
	/*if (Memindex == -1) {
		throw std::runtime_error("Error: No free frames available");
	}*/

	//mark all the memory space occupied by pid size as pid
	for (int i = Memindex; i < Memindex + size; i++) {
		memory[i] = pid;
	}
	//add the frame to the occupied frames list
	occupiedFrames.push_back(std::make_tuple(pid, Memindex, Memindex + size - 1));
}

void PagingAllocator::deallocate2(int pid) {
	std::unique_lock<std::shared_mutex> lock(memoryMutex);
	//find the pid in the occupiedFrames list
	int index = -1;
	for (int i = 0; i < occupiedFrames.size(); i++) {
		if (std::get<0>(occupiedFrames[i]) == pid) {
			index = i;
			break;
		}
	}
	//will never reach this point since memory is checked before calling this function
	/*if (index == -1) {
		throw std::runtime_error("Error: Process not found in memory");
	}*/

	//mark all the memory space occupied by pid size as -1
	for (int i = std::get<1>(occupiedFrames[index]); i <= std::get<2>(occupiedFrames[index]); i++) {
		memory[i] = -1;
	}
	//remove the frame from the occupied frames list
	occupiedFrames.erase(occupiedFrames.begin() + index);
}

void* PagingAllocator::allocate(size_t size) {
	
}

void PagingAllocator::deallocate(void* ptr) {

}