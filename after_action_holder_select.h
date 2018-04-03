#pragma once

#include "after_action_holder_default.h"
#include "TContainerProxy.h"

template<typename T1,
	typename T2,
	typename T3, typename RetValFromGui> class TAfterActionHolderSelect:public AfterActionHolderValue<std::decay_t<RetValFromGui>>
{
	using Type1 = std::decay_t<T1>;
	using Type2 = std::decay_t<T2>;
	using Type3 = std::decay_t<T3>;
	auto put(std::true_type, QDbInteractionThread* thread)
	{
		return thread->query(std::move(query), std::move(handler), std::move(errorHandler));
	}
	auto put(std::false_type, QDbInteractionThread* thread)
	{
		return thread->query(std::bind(std::move(query), std::move(proxy_value.value)), std::move(handler), std::move(errorHandler));
	}
public:
	TAfterActionHolderSelect(T1&& _query,
							 T2&& _handler,
							 T3&& _errorHandler) :query(std::forward<T1>(_query)),
		               handler(std::forward<T2>(_handler)),
		               errorHandler(std::forward<T3>(_errorHandler))
	{

	}
	virtual ~TAfterActionHolderSelect() = default;

	bool isCommandSet()const override
	{
		return true;
	}
	void putAfterCommand(QDbInteractionThread* thread)override
	{
		auto builder = put(std::is_void<RetValFromGui>(), thread);
		if (this->afterActionHolder())
		{
			builder.setTailFrom(this->afterActionHolder());
		}
	}


private:
	Type1 query;
	Type2 handler;
	Type3 errorHandler;
};
