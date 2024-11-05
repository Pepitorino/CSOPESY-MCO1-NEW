#pragma once
#include "TypeDefRepo.h"
class IMemoryAllocator
{
public:
	virtual void* allocate(size_t size) = 0;
	virtual void deallocate(void* ptr) = 0;
	virtual String visualizeMemory() = 0;
};

