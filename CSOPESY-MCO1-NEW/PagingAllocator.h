#pragma once
#include "IMemoryAllocator.h"
#include "TypeDefRepo.h"
//singleton as well
class PagingAllocator :
    public IMemoryAllocator
{
public:
    static void initialize(size_t maxMem, size_t frameSize);
    static void destroy();
    static PagingAllocator* getInstance();

	boolean IsMemoryAvailable(size_t size);
    boolean IsProcessInMemory(int pid);

    void* allocate(size_t size) override;
    void  deallocate(void* ptr) override;

	//Our version of allocate and deallocate
	void allocate2(int pid, size_t size);
	void deallocate2(int pid);
	String visualizeMemory() override;
    void visualizeMemory2(int coreId, u_int qqCycle);

	static std::shared_mutex memoryMutex; //shared mutex for memory, to be called by EVERYONE (who's using memory)

private:
    PagingAllocator(size_t maxMem, size_t frameSize);
    PagingAllocator();
    ~PagingAllocator() = default;
    PagingAllocator(PagingAllocator const&) {}; //copy constructor is private
    PagingAllocator& operator=(PagingAllocator const&) {}; //assignment operator is private
    static PagingAllocator* sharedAllocator;

    size_t maxMem;
    size_t frameSize;
    size_t allocatedMem;
    int numFrames;
	u_int visualizeCounter;
	
	std::vector <int> memory; // should include pid values. -1 is free, otherwise, it's the pid
	std::vector <std::tuple<int, int, int>> occupiedFrames; //pid, beginning of pid memory, end of pid memory
    //std::unordered_map<size_t, int> frameMap; //String is process, int is the page number
    //std::vector<size_t> freeFrameList;
};

