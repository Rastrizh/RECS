#ifndef EVENT_H
#define EVENT_H

#include <list>
#include <functional>
#include <utility>

namespace RECS {

template<class... TArgs>
class event
{
	using delegateType = std::function<void(TArgs...)>;
private:
	std::list<delegateType> m_delegateList;

public:
	void Connect(const delegateType& _delegate)
	{
		m_delegateList.push_back(delegateType(_delegate));
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

	bool operator==(const delegateType& rhs) const
	{
		return Hash() == rhs.Hash();
	}

private:
	size_t Hash(const delegateType& func)
	{
		return func.target_type().hash_code();
	}
};
}

#endif // !EVENT_H
