#ifndef CHUNK_ALLOCATOR_H
#define CHUNK_ALLOCATOR_H

#include "LinearAllocator.h"
#include "PoolAllocator.h"

namespace RECS { namespace memory {

#define CHUNK_SIZE 100

template<typename T>
class ChunkAllocator
{
public:
	const char* owner;
	LinearAllocator line;
	PoolAllocator pool;
public:
	ChunkAllocator(const char* _owner)
		:
		owner{ _owner },
		line{ sizeof(T) * KIBIBYTE },
		pool{ line.allocate(sizeof(T) * CHUNK_SIZE, alignof(T)), sizeof(T), CHUNK_SIZE }
	{
	}
	ChunkAllocator(size_t size, const char* _owner)
		:
		owner{ _owner },
		line{ size + alignof(T) },
		pool{ line.allocate(sizeof(T) * CHUNK_SIZE, alignof(T)), sizeof(T), CHUNK_SIZE }
	{
	}
	ChunkAllocator(void* start_ptr, size_t size, const char* _owner)
		:
		owner{ _owner },
		line{ start_ptr, size + alignof(T) },
		pool{ line.allocate(sizeof(T) * CHUNK_SIZE, alignof(T)), sizeof(T), CHUNK_SIZE }
	{
	}
	void* alloc(size_t size)
	{
		void* ret = pool.allocate(size, alignof(T));
		if (ret == nullptr)
		{
			//RINFO("Chunk allocator of {}", owner);
			//RINFO("Stats {} {}", "line: ", line.m_stats.ToString());
			//RINFO("Stats {} {}", "Pool: ", pool.m_stats.ToString());

			void* new_pool_start = line.allocate(sizeof(T) * CHUNK_SIZE, alignof(T));
			if (!new_pool_start)
				return ret;

			PoolAllocator new_pool(new_pool_start, sizeof(T), CHUNK_SIZE);
			pool.setHead(new_pool.getStartPtr());
			pool.m_stats += new_pool.m_stats;
			ret = pool.allocate(size, alignof(T));
		}
		return ret;
	}
	void dealloc(void* ptr) { ((T*)ptr)->~T(); pool.free(ptr); }
	size_t getBlockSize() { return pool.getBlockSize(); }

	T* operator[](size_t index) 
	{ 
		void* ret = pool[index];
		assert(ret && "Incorrect index, no data in that location"); 
		return (T*)ret; }

}; // class ChunkAllocator 

}} // namespace RECS::memory
#endif // !CHUNK_ALLOCATOR_H