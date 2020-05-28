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
	std::vector<std::future<void>> m_Futures;
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

	auto call_asunc(TArgs...params) const ->std::future<void>
	{
		return std::async(std::launch::async, [this](TArgs... asyncParams) { call(std::forward<TArgs>(asyncParams)...); }, std::forward<TArgs>(params)...);
	}

	auto operator +=(const delegateType& _delegate)->event&
	{
		Connect(_delegate);
		return *this;
	}
	
	auto operator -=(const delegateType& _delegate)->event&
	{
		Remove(_delegate);
		return *this;
	}

	inline void operator()(TArgs...args)
	{
		std::lock_guard<std::mutex> Lock(m_delegateLocker);
		m_Futures.push_back(call_asunc(std::forward<TArgs>(args)...));
	}

	auto operator==(const delegateType& rhs) const ->bool
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

	auto get_delegates_copy() const ->std::list<delegateType>
	{
		std::lock_guard<std::mutex> lock(m_delegateLocker);

		return m_delegateList;
	}

	auto Hash(const delegateType& func)->size_t
	{
		return func.target_type().hash_code();
	}
};
}

#endif // !EVENT_H
