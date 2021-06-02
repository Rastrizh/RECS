#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H

#include "Allocator.h"
#include <algorithm>

namespace RECS { namespace memory {
struct Header
{
	u8 adj;
};

class StackAllocator : public Allocator
{
private:
	void* m_start_ptr;
	size_t m_curr_offset;
public:
	StackAllocator()
		: Allocator(MEBIBYTE), m_curr_offset { 0 }
	{
		m_start_ptr = malloc(m_stats.total_size);
	}
	StackAllocator(size_t&& size)
		: Allocator(std::move(size)), m_curr_offset{ 0 }
	{
		m_start_ptr = malloc(m_stats.total_size);
	}
	StackAllocator(void* start_ptr, size_t size)
		: Allocator(std::move(size)), m_start_ptr{ start_ptr }, m_curr_offset{ 0 }
	{
	}
	virtual ~StackAllocator() override
	{

	}
	virtual void* allocate(size_t size, u8 align) override
	{
		if (m_curr_offset + size >= m_stats.total_size)
			return nullptr;
		uintptr_t curr_address = (uintptr_t)m_start_ptr + m_curr_offset;

		size_t adjustment = CalculateHeader(curr_address, align, sizeof(Header));
		m_curr_offset += size + adjustment;

		uintptr_t next_address = curr_address + adjustment;

		size_t header_address = next_address - sizeof(Header);
		Header* h_ptr = (Header*)header_address;
		(*h_ptr).adj = adjustment;

		updateStats(size + adjustment);
		return (void*)next_address;
	}

	virtual void free(void* ptr = nullptr) override
	{
		Header* header = (Header*)((size_t)ptr - sizeof(Header));
		m_curr_offset = (size_t)ptr - (size_t)header->adj - (size_t)m_start_ptr;
		m_stats.current_usage = m_curr_offset;
	}

	virtual void clear() override
	{
		m_curr_offset = 0;
		resetStats();
	}

	void* getStartPtr() { return m_start_ptr; }

private:
	size_t CalculateHeader(uintptr_t addr, uintptr_t align, uintptr_t header_size)
	{
		size_t adjustment = calculateAdjustment(addr, align);

		if (adjustment < header_size)
			return adjustment += align;
		return adjustment;
	}

};
}} // namespace RECS::memory

#endif // !STACK_ALLOCATOR_H