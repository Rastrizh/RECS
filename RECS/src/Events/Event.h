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
	using delegateType = std::function<void(TArgs...)>;
private:
	std::list<delegateType> m_delegateList;
	mutable std::mutex m_delegateLocker;

public:
	void Connect(const delegateType& _delegate)
	{
		std::lock_guard<std::mutex> lock(m_delegateLocker);

		m_delegateList.push_back(delegateType(_delegate));
	}
	
	void Remove(const delegateType& _delegate)
	{
		std::lock_guard<std::mutex> lock(m_delegateLocker);

		m_delegateList.remove_if([&](delegateType& _delegate_)
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

	std::future<void> call_asunc(TArgs...params) const
	{
		return std::async(std::launch::async, [this](TArgs... asyncParams) { call(std::forward<TArgs>(asyncParams)...); }, std::forward<TArgs>(params)...);
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
		call_asunc(std::forward<TArgs>(args)...);
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
