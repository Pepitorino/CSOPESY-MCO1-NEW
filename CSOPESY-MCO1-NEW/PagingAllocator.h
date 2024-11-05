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

    void* allocate(size_t size) override;
    void  deallocate(void* ptr) override;
    String visualizeMemory();

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
    std::vector<char> memory;
    std::vector<std::vector<char>> pages;
    std::unordered_map<String, int> pageTable; //String is process, int is the page number
};

