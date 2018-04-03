#pragma once

#include "QGuiSideDefaultEvent.h"
template<typename T1,typename T2>
class QCommandGuiSideEvent :public QGuiSideDefaultEvent<T2>
{
	using Type1 = std::decay_t<T1>;
	bool exec(std::false_type)
	{
		std::invoke(guiFunction);
		return true;
	}
	bool exec(std::true_type)
	{
		return false;
	}
	public:
		QCommandGuiSideEvent(T1&& _guiFunction,
							 T2&& errorFunction,
							 const std::shared_ptr<CAfterActionHolderDefault>& actionAfterEvent):
			QGuiSideDefaultEvent(std::forward<T2>(errorFunction), actionAfterEvent), guiFunction(std::forward<T1>(_guiFunction))
		{

		}
		bool execute()
		{
			return exec(std::is_null_pointer<Type1>());
		}
	private:
		Type1 guiFunction;
};
