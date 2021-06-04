#ifndef EVENT_H
#define EVENT_H

#include <list>
#include <functional>
#include <utility>
#include <future>

namespace RECS {
template<class... TArgs>
class event
{
private:
	using delegateType = std::function<void(TArgs...)>;
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

	void call(TArgs... params) const
	{
		std::list<delegateType> delegatesCopy = get_delegates_copy();

		call_impl(delegatesCopy, std::forward<TArgs>(params)...);
	}

	/*std::future<void>*/void call_asunc(TArgs...params) const
	{
		std::async(std::launch::async,
			&event::call, this, std::forward<TArgs>(params)...);
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
		/*m_Futures.push_back(*/call_asunc(std::forward<TArgs>(args)...);
	}

	bool operator==(const delegateType& rhs) const
	{
		return Hash() == rhs.Hash();
	}

private:
	void call_impl(const std::list<delegateType>& delegatesCopy, TArgs...params) const
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

	size_t Hash(const delegateType& func)
	{
		return func.target_type().hash_code();
	}
};
}

#endif // !EVENT_H