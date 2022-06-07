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
	std::list<std::future<void>> m_Futures;

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
	
	auto call_asunc(TArgs...params) const ->std::future<void>
	{
		return std::async(std::launch::async, &event::call_impl, &(*this), std::forward<TArgs>(params)...);
  }

	void call(TArgs... params) const
	{
		call_impl(std::forward<TArgs>(params)...);
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
		call_asunc(std::forward<TArgs>(args)...);
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
};
}

#endif // !EVENT_H