#include "MemoryAllocator.h"

MemoryAllocator* MemoryAllocator::sharedAllocator = nullptr;
std::shared_mutex MemoryAllocator::memoryMutex;

MemoryAllocator::MemoryAllocator(){
	
}

void MemoryAllocator::initialize() {
	sharedAllocator = new MemoryAllocator();
}

void MemoryAllocator::initializeMemory(size_t maxMem, size_t frameSize) {
	sharedAllocator->maxMem = maxMem; //since maxMem is 16384, it's 16KB
	sharedAllocator->frameSize = frameSize;
	sharedAllocator->numFrames = maxMem / frameSize;
	sharedAllocator->visualizeCounter = 0;
	if (sharedAllocator->numFrames <= 1) {
		sharedAllocator->allocator = MemoryAllocator::ALLOCATOR_TYPE::FLAT;
		for (int i = 0; i < sharedAllocator->maxMem; i++) { //one element per byte
			int x = -1; //if -1, that means its free
			sharedAllocator->flatMemory.push_back(x);
		}
		sharedAllocator->freeList.push_back(std::make_tuple(0,sharedAllocator->maxMem-1)); //the first contiguous memory is just the whole sapce, so from 0 to the maxMemory
	}
	else {
		sharedAllocator->allocator = MemoryAllocator::ALLOCATOR_TYPE::PAGING;
		for (int i = 0; i < sharedAllocator->numFrames; i++) {
			int frame = frameSize;
			sharedAllocator->freeFrameList.push_back(frameSize);
			sharedAllocator->frameMap[i] = -1;
		}
		sharedAllocator->numFreeFrames = sharedAllocator->numFrames;
	}

	//check if the folder for the file output exists, if not, create it
	if (sharedAllocator->doesFolderExist("Memory Visual Outputs")) {
		sharedAllocator->removeFolder("Memory Visual Outputs");
		sharedAllocator->createFolder("Memory Visual Outputs");
	}
	else sharedAllocator->createFolder("Memory Visual Outputs");

}

void MemoryAllocator::visualizeMemory(int Coreid, u_int qqCycle) {
	std::unique_lock<std::shared_mutex> lock(memoryMutex);
	//not implemented yet
	//output the .txt file to the 'Memory Visual Outputs' folder
	//file should be named 'Core(Coreid)_memory_stamp_(qqCycle).txt'
	//get current time
	std::vector <String> outputlines;
	time_t now = time(0);
	// Convert time_t to tm struct for local time
	struct tm localTime;
	localtime_s(&localTime, &now);

	// Create a buffer to hold the formatted time string
	char timeBuffer[80];
	strftime(timeBuffer, sizeof(timeBuffer), "%m/%d/%Y %I:%M:%S%p", &localTime);
	//std::cout << "Time Started: " << timeBuffer << std::endl;
	String timeBuffer2 = timeBuffer;
	String string_output = "Timestamp: (" + timeBuffer2 + ")";
	outputlines.push_back(string_output);

	if (this->allocator == MemoryAllocator::ALLOCATOR_TYPE::FLAT) {
		String no_process_inmemory = "Number of processes in memory: " + std::to_string(this->occupiedMemory.size());
		outputlines.push_back(no_process_inmemory);

		//use freeList to calculate the total free memory
		size_t totalFreeMemory = 0;
		for (auto t : this->freeList) {
			totalFreeMemory += std::get<1>(t) - std::get<0>(t) + 1;
		}
		String free_memory = "Total external fragmentation in KB: " + std::to_string(totalFreeMemory);
		outputlines.push_back(free_memory);

		String end, pid, start;
		end = "----end---- = " + std::to_string(this->maxMem);
		outputlines.push_back(end);
		for (int i = this->occupiedMemory.size() - 1; i >= 0; i--) {
			outputlines.push_back("");
			end = std::to_string(std::get<2>(this->occupiedMemory[i]));
			outputlines.push_back(end);
			pid = "PID: " + std::to_string(std::get<0>(this->occupiedMemory[i]));
			outputlines.push_back(pid);
			start = std::to_string(std::get<1>(this->occupiedMemory[i]));
			outputlines.push_back(start);
		}
		start = "----start---- = 0";
		outputlines.push_back(start);
	}
	else {
		//paging allocator, not implemented yet
	}
	String filename = "Memory Visual Outputs/Core" + std::to_string(Coreid) + "_memory_stamp_" + std::to_string(qqCycle) + ".txt";
	std::ofstream file(filename);
	for (String s : outputlines) {
		file << s << std::endl;
	}
	file.close();
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
			std::tuple<size_t, size_t> freeMem = this->freeList[i];
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

void MemoryAllocator::occupiedMemorySort() {
	std::sort(this->occupiedMemory.begin(), this->occupiedMemory.end(), [](const std::tuple<int, size_t, size_t>& a, const std::tuple<int, size_t, size_t>& b) {
		return std::get<1>(a) < std::get<1>(b);
		});
}

void MemoryAllocator::allocate(int pid, size_t size) {
	std::unique_lock<std::shared_mutex> lock(memoryMutex);
	if (this->allocator == MemoryAllocator::ALLOCATOR_TYPE::FLAT) {
		//check if theres memory available
		size_t index = this->IsMemoryAvailable(size, -1);
		//if there is, remove that memory from the freeList, set it in flatMemory, and add it to occupiedMemory
		if (index >= 0) {
			//look for it in the freeList, and just add the size to the starting index in the freeList
			//ex. if we add the first process, and it takes 200 bytes, the initial freeList of (0, maxMem-1) becomes
			//(200,maxMem-1) since 0-199 are now being used by the first process
			for (auto& t : this->freeList) {
				if (std::get<0>(t) == index) {
					std::get<0>(t) = (index + size);
				}
			}
			//occupiedMemory, we just push back the pid, the starting address, and ending address
			//in the same example as above, we'd push_back (0, 0, 199)
			this->occupiedMemory.push_back(std::make_tuple(pid,index,index+size-1));

			//sort the occupiedMemory vector by the starting address
			this->occupiedMemorySort();

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
				break;
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

bool MemoryAllocator::doesFolderExist(String folderName) {
	struct stat info;
	if (stat(folderName.c_str(), &info) != 0) return false;
	else if (info.st_mode & S_IFDIR) return true;
	else return false;
}

void MemoryAllocator::createFolder(String folderName) {
	String command = "mkdir \"" + folderName + "\"";
	system(command.c_str());
}

void MemoryAllocator::removeFolder(String folderName) {
	String command = "rm -rf \"" + folderName + "\"";
	system(command.c_str());
}