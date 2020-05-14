#ifndef DELEGATE_H
#define DELEGATE_H

#include <functional>
#include <utility>

namespace RECS {

template <typename... TArgs>
class delegate
{
	using functionType = std::function<void(TArgs...)>;
private:
	functionType m_func;
public:
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
		return Hash() == rhs.Hash();
	}

private:
	size_t Hash(const functionType& func)
	{
		return func.target_type().hash_code();
	}
};
}

#endif // !DELEGATE_H
