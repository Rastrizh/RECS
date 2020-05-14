#ifndef EVENT_H
#define EVENT_H

#include "Delegate.h"
#include <list>

namespace RECS {

template<class... TArgs>
class event
{
	using delegateType = delegate<TArgs...>;
private:
	std::list<delegateType> m_delegateList;

public:
	void Connect(const delegateType& _delegate)
	{
		m_delegateList.push_back(delegateType(_delegate));
	}

	void Connect(const std::function<void(TArgs...)>& _function)
	{
		m_delegateList.push_back(delegateType(_function));
	}

	void Remove(const delegateType& _delegate)
	{
		m_delegateList.remove_if([&](delegateType& _delegate_)
			{
				return _delegate == _delegate_;
			}
		);
	}

	event& operator +=(const delegateType& _delegate)
	{
		Connect(_delegate);
		return *this;
	}

	event& operator +=(const std::function<void(TArgs...)>& _function)
	{
		Connect(_function);
		return *this;
	}

	event& operator -=(const delegateType& _delegate)
	{
		Remove(_delegate);
		return *this;
	}

	inline void operator()(TArgs...args)
	{
		for (const auto& _delegate : m_delegateList)
		{
			_delegate(args...);
		}
	}
};
}

#endif // !EVENT_H
