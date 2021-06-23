#ifndef LINEAR_ALLOCATOR_H
#define LINEAR_ALLOCATOR_H

#include "Allocator.h"

namespace RECS { namespace memory {
class LinearAllocator : public Allocator
{
private:
	void* m_start_ptr;
	size_t m_curr_offset;

public:
	LinearAllocator()
		: Allocator(MEBIBYTE), m_curr_offset{ 0 }
	{
		m_start_ptr = malloc(m_stats.total_size);
	}
	LinearAllocator(size_t&& init_size)
		: Allocator(std::move(init_size)), m_curr_offset{ 0 }
	{
		m_start_ptr = malloc(m_stats.total_size);
	}
	LinearAllocator(void* start_ptr, size_t&& init_size)
		: Allocator(std::move(init_size)), m_start_ptr{ start_ptr }, m_curr_offset{ 0 }
	{
	}
	virtual ~LinearAllocator() override
	{
		free(m_start_ptr);
		m_start_ptr = nullptr;
		m_curr_offset = 0;
	}

	virtual void* allocate(const size_t& size, u8 alignment) override
	{
		if (m_curr_offset + size > m_stats.total_size)
			return nullptr;
		size_t curr_address = (size_t)m_start_ptr + m_curr_offset;
		size_t adjustment = calculateAdjustment(curr_address, alignment);
		curr_address += adjustment;
		m_curr_offset += size + adjustment;
		updateStats(size);
		return (void*)curr_address;
	}
	virtual void free(void* ptr) override {/* Linear allocator can anly free memory all at once */ }
	virtual void clear() override
	{
		m_curr_offset = 0;
		resetStats();
	}
	void* getStartPtr() const { return m_start_ptr; }
};
}} // namespace RECS::memory

#endif // !LINEAR_ALLOCATOR_H
