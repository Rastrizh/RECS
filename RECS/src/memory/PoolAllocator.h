#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include "Allocator.h"

namespace RECS { namespace memory {

class PoolAllocator : public Allocator
{
private:
	void* m_start_ptr;
	void* head;
	size_t m_element_size;
public:
	PoolAllocator(size_t elem_size, size_t num_elems)
		: Allocator(num_elems * elem_size), m_element_size{ elem_size }
{
	if (m_element_size < sizeof(size_t))
	{
		m_element_size = sizeof(size_t);
		m_stats.total_size = m_element_size * num_elems;
	}
	m_start_ptr = malloc(num_elems * m_element_size);
	head = m_start_ptr;
	initialize();
}

PoolAllocator(void* start_ptr, size_t elem_size, size_t num_elems)
	: Allocator(num_elems * elem_size), m_start_ptr{ start_ptr }, m_element_size{ elem_size }
{
	if (m_element_size < sizeof(size_t))
	{
		m_element_size = sizeof(size_t);
		m_stats.total_size = m_element_size * num_elems;
	}
	head = m_start_ptr;
	initialize();
}

PoolAllocator(const PoolAllocator& other)
	: Allocator(other.m_stats.total_size), m_start_ptr{ other.m_start_ptr }, head{ other.head }, m_element_size{ other.m_element_size }
{
}
PoolAllocator& operator=(const PoolAllocator& other)
{
	if (&other == this)
		return *this;
	m_stats += other.m_stats;
	m_start_ptr = other.m_start_ptr;
	head = other.head; 
	m_element_size =other.m_element_size;
	return *this;
}

PoolAllocator(PoolAllocator&& other)
	: Allocator(other.m_stats.total_size), m_start_ptr{ other.m_start_ptr }, head{ other.head }, m_element_size{ other.m_element_size }
{
}
PoolAllocator& operator=(PoolAllocator&& other)
{
	if (&other == this)
		return *this;
	m_stats += other.m_stats;
	m_start_ptr = other.m_start_ptr;
	head = other.head;
	m_element_size = other.m_element_size;
	return *this;
}

virtual ~PoolAllocator() override
{
	clear();
}

void* allocate(size_t size, u8 alignment) override
{
	if (size + m_stats.current_usage > m_stats.total_size)
		return nullptr;
	void* free_block = head;

	size_t count = size / m_element_size;
	for (int i = 0; i < count; i++)
	{ 
 		head = *(void**)head;
	}
	if (!head)
		return nullptr;

	updateStats(size);
	return free_block;
}

void free(void* ptr) override
{
	*(void**)ptr = head;
	head = ptr;
}

void clear() override
{
	//free(m_start_ptr);
	resetStats();
}

size_t getBlockSize() { return m_element_size; }
void* getStartPtr() { return m_start_ptr; }
void* getHead() { return head; }

void setHead(void* ptr) { head = ptr; }

private:
	void initialize()
	{
		size_t n_elems = m_stats.total_size / m_element_size;
		for (size_t i = 1; i <= n_elems; i++)
		{
			void* new_node = (void*)((size_t)m_start_ptr + m_element_size * i);
			*(void**)head = new_node;
			head = new_node;
		}
		*(void**)head = nullptr;
		head = m_start_ptr;
	}
};
}} // namespace RECS::memory

#endif // !POOL_ALLOCATOR_H