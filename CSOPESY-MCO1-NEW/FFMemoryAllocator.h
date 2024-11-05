#pragma once
#include "IMemoryAllocator.h"
#include "TypeDefRepo.h"
//singleton as well
class FFMemoryAllocator :
    public IMemoryAllocator
{
public:
    static void initialize(size_t maxMem, size_t frameSize);
    static void destroy();
    static FFMemoryAllocator* getInstance();

    void* allocate(size_t size) override;
    void  deallocate(void* ptr) override;
    String visualizeMemory();

private:
    FFMemoryAllocator(size_t maxMem, size_t frameSize);
    FFMemoryAllocator();
    ~FFMemoryAllocator() = default;
    FFMemoryAllocator(FFMemoryAllocator const&) {}; //copy constructor is private
    FFMemoryAllocator& operator=(FFMemoryAllocator const&) {}; //assignment operator is private
    static FFMemoryAllocator* sharedAllocator;

    size_t maxMem;
    size_t frameSize;
    size_t allocatedMem;
    int numFrames;
    std::vector<char> memory;
    std::vector<std::vector<char>> pages;
    std::unordered_map<String, int> pageTable; //String is process, int is the page number
};

