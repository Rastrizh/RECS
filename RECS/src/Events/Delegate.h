#ifndef DELEGATE_H
#define DELEGATE_H

#include <utility>

namespace RECS {

template <typename... TArgs>
class delegate
{
	using functionType = void (*)(TArgs...);
public:
	functionType m_func;
public:
	delegate() = default;
	explicit delegate(const functionType& func)
		:
		m_func(func)
	{ }

	delegate(const delegate& other)
		:
		m_func(other.m_func)
	{ }

	delegate(delegate&& other)
		:
		m_func(std::move(other.m_func))
	{ }

	delegate& operator=(const delegate& rhs)
	{
		m_func = rhs.m_func;
		return *this;
	}

	delegate& operator=(delegate&& rhs)
	{
		std::swap(m_func, rhs.m_func);
		return *this;
	}

	void operator()(TArgs... params) const
	{
		if (m_func)
			m_func(params...);
	}

	operator bool() const
	{
		return m_func;
	}

	bool operator==(const delegate& rhs) const
	{
		return m_func == rhs.m_func;
	}
};
}

#endif // !DELEGATE_H
