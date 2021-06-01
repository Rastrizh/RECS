#include "Allocator.h"

namespace RECS {
size_t memory::calculateAdjustment(uintptr_t address, uintptr_t align)
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