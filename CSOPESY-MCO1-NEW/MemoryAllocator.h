#pragma once
#include "TypeDefRepo.h"
class MemoryAllocator
{
public:
    enum ALLOCATOR_TYPE {
        FLAT,
        PAGING
    };
    static void initialize(size_t maxMem, size_t frameSize);
    void initializeMemory();
    static void destroy();
    static MemoryAllocator* getInstance();

    int IsMemoryAvailable(size_t size, int numFrames);  //numFrames should be -1 if flat allocator
                                                        //returns 0 for paging allocator if its true
    boolean IsProcessInMemory(int pid);

    void allocate(size_t size, int pid);
    void deallocate(int pid);
    void mergeFlatMemory();

    static std::shared_mutex memoryMutex;
private:
    MemoryAllocator(size_t maxMem, size_t frameSize);
    MemoryAllocator();
    ~MemoryAllocator() = default;
    MemoryAllocator(MemoryAllocator const&) {}; //copy constructor is private
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
    std::vector<std::tuple<int, int, int>> occupiedMemory; //pid, beginning of pid memory, end of pid memory
    std::vector<std::tuple<int, int>> freeList; //starting address, ending address of free contiguous memory, updated upon deallocating
                                                //used to find free address faster

    //paging allocator
    std::vector<int> freeFrameList;
    std::unordered_map<int, int> frameMap; //frame, pid 
    size_t numFreeFrames;
};

