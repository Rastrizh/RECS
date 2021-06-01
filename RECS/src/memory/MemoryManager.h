#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include "StackAllocator.h"
#include "../core/Log.h"
#include <assert.h>
#include <vector>

namespace RECS { namespace memory {

#define GLOBAL_MEMORY_SIZE 134217728

class MemoryManager
{
	struct MemoryUser
	{
		MemoryUser(const char* _name, size_t _size)
			: name(_name), size(_size)
		{ }
		const char* name;
		size_t size;
	};
private:
	static StackAllocator s_global_memory_allocator;
	static std::vector<MemoryUser> memory_users;
public:
	static void* NewMemoryUser(const char* name, size_t mem_size)
	{
		//assert((memory_users[0].name != name || memory_users[1].name != name) && "Trying to add existing memory user");
		assert(mem_size <= s_global_memory_allocator.m_stats.total_size && "Not enough memory");

		memory_users.emplace_back(name, mem_size);

		void* p = s_global_memory_allocator.allocate(mem_size, alignof(size_t));
		
		//RINFO("New memory user {} {} {}", name, ": ", mem_size);

		return p;
	}

	static void DeleteMemoryUser(char* name, void* p_user)
	{
		s_global_memory_allocator.free(p_user);
	}
};

std::vector<MemoryManager::MemoryUser> MemoryManager::memory_users;

StackAllocator MemoryManager::s_global_memory_allocator(GLOBAL_MEMORY_SIZE);

}} // namespace RECS::memory

#endif // !MEMORY_MANAGER_H