#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "../RECSTypes.h"
#include <iostream>
#include <string>
#include <sstream>

#define KIBIBYTE 1024
#define MEBIBYTE 1048576
#define GIBIBYTE 1073741824

namespace RECS { namespace memory {

size_t calculateAdjustment(const uintptr_t& address, const uintptr_t& align);

struct AllocatorStats
{
	size_t total_size;
	size_t current_usage;
	size_t num_allocations;

	AllocatorStats& operator+=(const AllocatorStats& other)
	{
		total_size += other.total_size;
		current_usage += other.current_usage;
		num_allocations += other.num_allocations;
		return *this;
	}
	std::string ToString() const
	{
		std::stringstream ss;
		ss << "Total allocated memory: " << total_size
			<< "; in use: " << current_usage
			<< "; number of allocations: " << num_allocations;
		return ss.str();
	}
	friend std::ostream& operator<<(std::ostream& out, const memory::AllocatorStats& stats);
};

class Allocator
{
public:
	AllocatorStats m_stats;
public:
	Allocator(size_t&& size) : m_stats{ size, 0, 0 } {}
	virtual ~Allocator() { m_stats = { 0,0,0 }; }

	virtual void* allocate(size_t size, u8 alignment) = 0;
	virtual void free(void* ptr) = 0;
	virtual void clear() = 0;

	void updateStats(size_t size) 
	{ 
		m_stats.current_usage += size;
		m_stats.num_allocations++;
	}
	void resetStats()
	{
		m_stats.total_size = 0;
		m_stats.current_usage = 0;
		m_stats.num_allocations = 0;
	}
};
}} // RECS::memory

#endif // !ALLOCATOR_H
