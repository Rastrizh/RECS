#ifndef EVENT_H
#define EVENT_H

#include "Delegate.h"
#include <list>
#include <functional>
#include <utility>
#include <future>

namespace RECS {
template<class... TArgs>
class event
{
private:
	using delegateType = delegate<TArgs...>;
	std::list<delegateType> m_delegateList;
	mutable std::mutex m_delegateLocker;
public:
	std::vector<std::future<void>> m_Futures;

public:
	void Connect(const delegateType& _delegate)
	{
		std::lock_guard<std::mutex> lock(m_delegateLocker);

		m_delegateList.push_back(delegateType(_delegate));
	}
	
	void Remove(const delegateType& _delegate)
	{
		std::lock_guard<std::mutex> lock(m_delegateLocker);

		m_delegateList.remove_if([&](const delegateType& _delegate_)
			{
				return _delegate == _delegate_;
			}
		);
	}

	void call(TArgs... params) const
	{
		call_impl(std::forward<TArgs>(params)...);
	}

	std::future<void> call_asunc(TArgs...params) const
	{
		return std::async(std::launch::async, &event::call, this, std::forward<TArgs>(params)...);
	}

	event& operator +=(void(*func)(TArgs...))
	{
		Connect(delegateType(func));
		return *this;
	}
	
	event& operator -=(void(*func)(TArgs...))
	{
		Remove(delegateType(func));
		return *this;
	}

	inline void operator()(TArgs...args)
	{
		m_Futures.push_back(call_asunc(std::forward<TArgs>(args)...));
	}

	bool operator==(const delegateType& rhs) const
	{
		return delegateType == rhs.delegateType;
	}

private:
	void call_impl(TArgs...params) const
	{
		for (const auto& _delegate : m_delegateList)
		{
			_delegate(std::forward<TArgs>(params)...);
		}
	}

	std::list<delegateType> get_delegates_copy() const
	{
		std::lock_guard<std::mutex> lock(m_delegateLocker);

		return m_delegateList;
	}
};
}

#endif // !EVENT_H