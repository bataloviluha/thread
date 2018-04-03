#pragma once

#include "QGuiSideEvent.h"
#include <functional>
template<typename T1>
class QGuiSideDefaultEvent :public QGuiSideEvent
{
	using TypeError = std::decay_t<T1>;
	bool exec(std::false_type)
	{
		std::invoke(errorFunction);
		return true;
	}
	bool exec(std::true_type)
	{
		return false;
	}
	public:
		QGuiSideDefaultEvent(T1&& _errorFunction = nullptr,
							 const std::shared_ptr<CAfterActionHolderDefault>& _actionAfterEvent = nullptr):
			errorFunction(std::forward<T1>(_errorFunction)), 
			afterEvent(_actionAfterEvent)
		{

		}
		virtual ~QGuiSideDefaultEvent() = default;

		bool executeError()override
		{
			return exec(std::is_null_pointer<TypeError>());
		}
		std::exception_ptr exceptionInfo()override
		{
			return exeptionPtr;
		}
		std::shared_ptr<CAfterActionHolderDefault> actionAfterEvent()override
		{
			return afterEvent;
		}

		void setExceptionPtr(std::exception_ptr&& _exeptionPtr)
		{
			exeptionPtr = std::move(_exeptionPtr);
		}
	private:
		TypeError errorFunction;
		std::exception_ptr exeptionPtr;
		std::shared_ptr<CAfterActionHolderDefault> afterEvent;
};
