#include "Allocator.h"

namespace RECS {
size_t memory::calculateAdjustment(const uintptr_t& address, const uintptr_t& align)
{
	uintptr_t adjust = address % align;
	if (adjust != 0)
		adjust = align - adjust;
	return adjust;
}

std::ostream& operator<<(std::ostream& out, const memory::AllocatorStats& stats)
{
	return out << stats.ToString();
}
} // namespace RECS