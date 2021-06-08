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
	PoolAllocator(size_t&& elem_size, size_t&& num_elems)
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

PoolAllocator(void* start_ptr, size_t&& elem_size, size_t&& num_elems)
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
	m_stats.current_usage -= m_element_size;
	m_stats.num_allocations--;
}

void clear() override
{
	//free(m_start_ptr);
	resetStats();
}

const size_t& getBlockSize() const { return m_element_size; }
void* getStartPtr() const { return m_start_ptr; }
void* getHead() const { return head; }
void setHead(void* ptr) { head = ptr; }

void* operator[](size_t index)
{
	return (m_element_size * index) < m_stats.total_size ? (void*)((uintptr_t)m_start_ptr + (uintptr_t)(m_element_size * index)) : nullptr;
}

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
