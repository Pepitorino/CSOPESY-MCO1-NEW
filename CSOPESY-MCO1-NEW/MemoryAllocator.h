#pragma once
#include "TypeDefRepo.h"

class MemoryAllocator
{
public:
    enum ALLOCATOR_TYPE {
        FLAT,
        PAGING
    };
    static void initialize();
    static void initializeMemory(size_t maxMem, size_t frameSize);
    static void destroy();
    static MemoryAllocator* getInstance();

    int IsMemoryAvailable(size_t size); //returns 0 for paging allocator if its true
    boolean IsProcessInMemory(int pid);

    void allocate(int pid, size_t size);
    void deallocate(int pid);
    void mergeFlatMemory();
    void visualizeMemory(u_int qqCycle);

    static std::shared_mutex memoryMutex;
private:
    MemoryAllocator();
    ~MemoryAllocator() = default;
    MemoryAllocator(MemoryAllocator const&) {}; //copy constructor is private

	bool doesFolderExist(String folderName);
	void createFolder(String folderName);
	void removeFolder(String folderName);

    void occupiedMemorySort();
    MemoryAllocator& operator=(MemoryAllocator const&) {}; //assignment operator is private
    static MemoryAllocator* sharedAllocator;

    size_t maxMem;
    size_t frameSize;
    size_t allocatedMem;
    int numFrames;
    u_int visualizeCounter;
    ALLOCATOR_TYPE allocator;

    //flat allocator
    std::vector<int> flatMemory; //-1 if no pid is there, each element represents a byte
    std::vector<std::tuple<int, size_t, size_t>> occupiedMemory; //pid, beginning of pid memory, end of pid memory
    std::vector<std::tuple<size_t, size_t>> freeList; //starting address, ending address of free contiguous memory, updated upon deallocating
                                                //used to find free address faster

    //paging allocator
    std::deque<int> freeFrameList; //index of free frame on frameMap
    std::unordered_map<int, int> frameMap; //frame, pid 
    size_t numFreeFrames;
    std::unordered_map<int, std::vector<int>> pidFrames; //pid, [frames taken]
    std::vector<int> processes; //pids
};

