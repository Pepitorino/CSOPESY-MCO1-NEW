#include "MemoryAllocator.h"
#include "ConsoleManager.h"
#include "Scheduler.h"

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
	sharedAllocator->NumPagedIn = 0;
	sharedAllocator->NumPagedOut = 0;
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
			sharedAllocator->freeFrameList.push_back(i);
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

void MemoryAllocator::destroy() {
	delete sharedAllocator;
}

MemoryAllocator* MemoryAllocator::getInstance() {
	return sharedAllocator;
}

//-1 for false;
//use numFrames == -1 for FlatAllocator
int MemoryAllocator::IsMemoryAvailable(size_t size) {
	std::shared_lock<std::shared_mutex> lock(memoryMutex);
	if (this->allocator == MemoryAllocator::ALLOCATOR_TYPE::FLAT) {
		for (size_t i = 0; i < this->freeList.size(); i++) {
			if (std::get<0>(this->freeList[i]) >= std::get<1>(this->freeList[i])) {
				this->freeList.erase(this->freeList.begin() + i);
				if (i >= this->freeList.size()) return -1;
			}
			std::tuple<size_t, size_t> freeMem = this->freeList[i];
			if (std::get<1>(freeMem) - std::get<0>(freeMem) >= size-1) {
				return std::get<0>(freeMem);
			}
		}
	}
	else {
		int freeFrames = (this->frameSize>=size) ? 1 : (size+1 / this->frameSize);
		if (this->numFreeFrames >= freeFrames) return 0;
	}
	return -1;
}

boolean MemoryAllocator::IsProcessInMemory(int pid) {
	std::shared_lock<std::shared_mutex> lock(memoryMutex);
	if (this->allocator == MemoryAllocator::ALLOCATOR_TYPE::FLAT) {
		for (auto t : this->occupiedMemory) {
			if (std::get<0>(t) == pid) return true;
		}
	}
	else {
		for (auto x : this->pidFrames) {
			if (x.first == pid) return true;
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
	std::shared_lock<std::shared_mutex> lock(memoryMutex);
	if (this->allocator == MemoryAllocator::ALLOCATOR_TYPE::FLAT) {
		//check if theres memory available
		size_t index = this->IsMemoryAvailable(size);

		//ready or not here i come
		while (this->IsMemoryAvailable(size) < 0) {
			auto t = this->occupiedMemory.front();
			auto p = ConsoleManager::getInstance()->getProcess(std::get<0>(t));
			while (true) {
				if (p->state == Process::process_state::READY) break;
			}
			this->addToBackingStore(std::get<0>(t));
			this->deallocate(std::get<0>(t));
		}

		if (this->inBackingStore(pid)) this->removeFromBackingStore(pid);

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
			this->occupiedMemory.push_back(std::make_tuple(pid, index, index + size - 1));

			//sort the occupiedMemory vector by the starting address
			this->occupiedMemorySort();

			//just sets the memory to the pid
			for (index; index < size; index++) {
				flatMemory[index] = pid;
			}


		}
	}
	else {
		size_t numFrames = (this->frameSize >= size) ? 1 : (size + 1 / this->frameSize);

		while (this->IsMemoryAvailable(size) < 0) {
			auto i = this->processes.front();
			auto p = ConsoleManager::getInstance()->getProcess(i);
			while (true) {
				if (p->state == Process::process_state::READY) break;
			}
			this->addToBackingStore(i);
			this->deallocate(i);
		}

		if (this->inBackingStore(pid)) this->removeFromBackingStore(pid);

		if (!(this->IsMemoryAvailable(size))) {
			std::vector<int> framesUsed;
			for (int i = 0; i < numFrames; i++) {
				int freeFrame = this->freeFrameList.front();
				this->freeFrameList.pop_front();
				this->frameMap[freeFrame] = pid;
				framesUsed.push_back(freeFrame);
			}
			this->pidFrames[pid] = framesUsed;
			this->numFreeFrames -= framesUsed.size();
			this->processes.push_back(pid);
			this->NumPagedIn += framesUsed.size();
		}
	}
}

void MemoryAllocator::deallocate(int pid) {
	std::shared_lock<std::shared_mutex> lock(memoryMutex);

	if (this->inBackingStore(pid)) this->removeFromBackingStore(pid);

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
		std::vector<int> numFrames = this->pidFrames[pid];
		this->pidFrames.erase(pid);

		for (auto it = this->processes.begin(); it != this->processes.end();) {
			if (*it == pid) {
				it = this->processes.erase(it);
				break;
			}
			else {
				++it;
			}
		}

		for (int i = 0; i < numFrames.size(); i++) {
			frameMap[numFrames[i]] = -1;
			this->freeFrameList.push_back(numFrames[i]);
		}
		this->numFreeFrames += numFrames.size();
		this->NumPagedOut += numFrames.size();
		//paging allocator, not implemented yet
		//idea is to set the frameMap frames to -1, then add those to freeFrameList
	}
}

void MemoryAllocator::mergeFlatMemory() {
	//basically just changes the start to the end or vice versa type beat
	for (int i = 0; i < this->freeList.size(); i++) {
		for (int j = i+1; j < this->freeList.size(); j++) {
			size_t start1 = std::get<0>(freeList[i]),
				end1 = std::get<1>(freeList[i]);
			size_t start2 = std::get<0>(freeList[j]),
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

std::vector<String> MemoryAllocator::processSmi() {
	//lock memory manager
	std::shared_lock<std::shared_mutex> lock(memoryMutex);

	//should lock all CPUs here
	std::vector<std::shared_ptr<CPUSerf>> cpuListCManager = Scheduler::getInstance()->giveCPUs();
	std::vector<std::unique_lock<std::shared_mutex>> locks;
	int numCPUs = cpuListCManager.size();
	locks.reserve(numCPUs);

	//lock all CPUs
	for (int i = 0; i < numCPUs; i++) {
		locks.push_back(std::unique_lock<std::shared_mutex>(cpuListCManager.at(i)->CPUMutex));
	}

	std::unique_lock<std::shared_mutex> lockglobal(ConsoleManager::processListMutex);

	std::vector<String> strings;
	std::tuple<float, int, int> coredetails = Scheduler::getInstance()->findCoresUsed();
	std::ostringstream corestrstream;
	corestrstream << "CPU Utilization: " << std::fixed << std::setprecision(2) << std::get<0>(coredetails) << "%\n";
	if (this->allocator == MemoryAllocator::ALLOCATOR_TYPE::FLAT) {
		size_t memUsed = 0;
		for (auto t : occupiedMemory) {
			memUsed += std::get<2>(t) - std::get<1>(t) + 1;
		}
		corestrstream << "Memory Usage: " << memUsed << "/" << this->maxMem << "\n";
		corestrstream << "Memory Util: " << (float)((float)memUsed / (float)this->maxMem)*100 << "%\n";
	}
	else {
		size_t memUsed = maxMem - this->freeFrameList.size()*this->frameSize;
		corestrstream << "Memory Usage: " << memUsed << "/" << this->maxMem << "\n";
		corestrstream << "Memory Util: " << (float)((float)memUsed / (float)this->maxMem)*100 << "%\n";
	}
	strings.push_back(corestrstream.str());

	strings.push_back("===========================================\n");
	strings.push_back("Running Processes and Memory Usage:\n");
	strings.push_back("===========================================\n");

	//std::shared_ptr<std::vector<std::shared_ptr<Process>>> processList = ConsoleManager::getInstance()->giveProcess_InOrderVectorToScheduler();

	for (int i = 0; i < numCPUs; i++) {
		std::ostringstream procstream;
		std::shared_ptr<Process> process = cpuListCManager.at(i)->WhatIsYourWork_Slave();
		if (process != nullptr) {
			procstream << std::left << std::setw(15) << process->getName() << process->getMemorySize() << "\n";
			strings.push_back(procstream.str());
		}
	}

	return strings;
}

std::vector<String> MemoryAllocator::vmstat() {
	//total mem, used mem, free mem, idle cpu ticks, active cpu ticks, total cpu ticks, num paged in, num paged out

	//should lock all CPUs here
	std::vector<std::shared_ptr<CPUSerf>> cpuListMemManager = Scheduler::getInstance()->giveCPUs();
	std::vector<std::unique_lock<std::shared_mutex>> locks;
	int numCPUs = cpuListMemManager.size();
	locks.reserve(numCPUs);

	//lock all CPUs
	for (int i = 0; i < numCPUs; i++) {
		locks.push_back(std::unique_lock<std::shared_mutex>(cpuListMemManager.at(i)->CPUMutex));
	}

	std::vector<String> strings;
	std::ostringstream memstream;
	memstream << "Total Memory: " << this->maxMem << " KB\n";
	strings.push_back(memstream.str());

	std::ostringstream memstream2;
	if (this->allocator == MemoryAllocator::ALLOCATOR_TYPE::FLAT) {
		size_t memUsed = 0;
		for (auto t : occupiedMemory) {
			memUsed += std::get<2>(t) - std::get<1>(t) + 1;
		}
		memstream2 << "Used Memory: " << memUsed << " KB\n";
		//free memory
		memstream2 << "Free Memory: " << this->maxMem - memUsed << " KB\n";
	}
	else {
		size_t memUsed = maxMem - this->freeFrameList.size() * this->frameSize;
		memstream2 << "Memory Usage: " << memUsed << " KB\n";
		memstream2 << "Free Memory: " << this->maxMem - memUsed << " KB\n";
	}
	strings.push_back(memstream2.str());


	std::ostringstream cpustream;
	//idle cpu ticks, active cpu ticks, total cpu ticks
	uint64_t idle = 0, active = 0, total = 0;
	for (int i = 0; i < numCPUs; i++) {
		idle += cpuListMemManager.at(i)->HowLongYouBeenSlackin();
		active += cpuListMemManager.at(i)->HowLongYouBeenInDaFields();
		total += cpuListMemManager.at(i)->HowLongYouveBeenPoor();
	}

	cpustream << "Idle CPU Ticks: " << idle << "\n";
	cpustream << "Active CPU Ticks: " << active << "\n";
	cpustream << "Total CPU Ticks: " << total << "\n";
	strings.push_back(cpustream.str());

	//needs rework
	std::ostringstream pagedstream;
	pagedstream << "Number of Pages Paged In: " << this->NumPagedIn << "\n";
	pagedstream << "Number of Pages Paged Out: " << this->NumPagedOut << "\n";
	strings.push_back(pagedstream.str());

	return strings;
}

void MemoryAllocator::addToBackingStore(int pid) {
	std::shared_ptr<Process> process = ConsoleManager::getInstance()->getProcess(pid);
	this->backingStore[pid] = std::make_tuple(process->getName(), process->getMemorySize(), -1, process->getProcessProgress(), process->getLines());
}
void MemoryAllocator::addToBackingStore(int pid, std::vector<int> frames) {
	std::shared_ptr<Process> process = ConsoleManager::getInstance()->getProcess(pid);
	this->backingStore[pid] = std::make_tuple(process->getName(), process->getMemorySize(), frames.size(), process->getProcessProgress(), process->getLines());
}
void MemoryAllocator::removeFromBackingStore(int pid) {
	this->backingStore.erase(pid);
}
bool MemoryAllocator::inBackingStore(int pid) {
	return backingStore.find(pid) != backingStore.end();
}
void MemoryAllocator::printBackingStore() {
	//do the 
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
		outputlines.push_back("");
		start = "----start---- = 0";
		outputlines.push_back(start);
	}
	else {
		String no_process_inmemory = "Number of processes in memory: " + std::to_string(this->occupiedMemory.size());
		outputlines.push_back(no_process_inmemory);
		String free_memory = "Total pages used: " + std::to_string(numFrames - numFreeFrames);
		outputlines.push_back(free_memory);
		String end, pid, start;
		end = "----end---- = " + std::to_string(this->numFrames);
		outputlines.push_back(end);
		for (int i = this->frameMap.size() - 1; i >= 0; i--) {
			outputlines.push_back("");
			end = std::to_string(i);
			outputlines.push_back(end);
			int framePid = this->frameMap[i];
			if (framePid >= 0) {
				pid = "PID: " + std::to_string(this->frameMap[i]);
			}
			else {
				pid = "N/A";
			}
			outputlines.push_back(pid);
		}
		outputlines.push_back("");
		start = "----start---- = 0";
		outputlines.push_back(start);
	}
	outputlines.push_back("\n\n==============================================\n");
	std::vector<String> backingStoreLines;
	if (this->allocator == MemoryAllocator::ALLOCATOR_TYPE::FLAT) {
		outputlines.push_back("Processes in Backing Store: \n");
		for (auto it = backingStore.begin(); it != backingStore.end(); ++it) {
			int key = it->first;
			auto& value = it->second;
			std::ostringstream process;
			process << "\n" << std::get<0>(value) << "\n";
			process << "\tPID: " << key << "\n";
			process << "\tMemory Size: " << std::get<1>(value) << "\n";
			process << "\tCommands Done: " << std::get<3>(value) << "\n";
			process << "\tCommands Left: " << std::get<4>(value) << "\n";
			backingStoreLines.push_back(process.str());
		}
	}
	else {
		outputlines.push_back("Processes in Backing Store: \n");
		for (auto it = backingStore.begin(); it != backingStore.end(); ++it) {
			int key = it->first;
			auto& value = it->second;
			std::ostringstream process;
			process << "\n" << std::get<0>(value) << "\n";
			process << "\tPID: " << key << "\n";
			process << "\tMemory Size: " << std::get<1>(value) << "\n";
			process << "\tPages Used: " << std::get<2>(value) << "\n";
			process << "\tCommands Done: " << std::get<3>(value) << "\n";
			process << "\tCommands Left: " << std::get<4>(value) << "\n";
			backingStoreLines.push_back(process.str());
		}
	}
	outputlines.insert(outputlines.end(), backingStoreLines.begin(), backingStoreLines.end());
	outputlines.push_back("\n\n==============================================\n");
	String filename = "BackingStore.txt";
	std::ofstream file(filename);
	for (String s : outputlines) {
		file << s << std::endl;
	}
	file.close();
}