#ifndef CHUNK_ALLOCATOR_H
#define CHUNK_ALLOCATOR_H

#include "LinearAllocator.h"
#include "PoolAllocator.h"

namespace RECS { namespace memory {

#define CHUNK_SIZE 100

template<typename T>
class ChunkAllocator
{
private:
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
	ChunkAllocator(size_t&& size, const char* _owner)
		:
		owner{ _owner },
		line{ std::move(size) },
		pool{ line.allocate(sizeof(T) * CHUNK_SIZE, alignof(T)), sizeof(T), CHUNK_SIZE }
	{
	}
	ChunkAllocator(void* start_ptr, size_t&& size, const char* _owner)
		:
		owner{ _owner },
		line{ start_ptr, std::move(size) },
		pool{ line.allocate(sizeof(T) * CHUNK_SIZE, alignof(T)), sizeof(T), CHUNK_SIZE }
	{
	}
	void* alloc()
	{
		void* ret = pool.allocate(pool.getBlockSize(), alignof(T));
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
			pool.setStats(new_pool.getStats());
			ret = pool.allocate(pool.getBlockSize(), alignof(T));
		}
		return ret;
	}
	void dealloc(void* ptr) { ((T*)ptr)->~T(); pool.free(ptr); }

	T* operator[](size_t index) 
	{ 
		assert(index <= line.TotalSize() / pool.getBlockSize() && "Incorrect index, no data in that location"); 
		void* ret = (void*)((uintptr_t)line.getStartPtr() + pool.getBlockSize() * index);
		return (T*)ret; 
	}

	void clear() { line.clear(); }

	const PoolAllocator& getPool() const { return pool; }
	const LinearAllocator& getLine() const { return line; }
	const char* getOwner() const { return owner; }

	const size_t& getBlockSize() const { return pool.getBlockSize(); }
	const AllocatorStats& getLineStats() const { return line.getStats(); }
	const AllocatorStats& getPoolStats() const { return pool.getStats(); }
}; // class ChunkAllocator 

}} // namespace RECS::memory
#endif // !CHUNK_ALLOCATOR_H