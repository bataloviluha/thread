#pragma once

#include "QGuiSideDefaultEvent.h"
#include "TContainerProxy.h"

template<typename T1,///<��� ������� �������������� �� ���������� ����� 
	typename T2,///<��� ������� �������������� ������ ���������� 
	typename TContainerType///<��� ���������� ������������� � ������� 
		 > class QValueGuiSideEvent:public QGuiSideDefaultEvent<T2>
{
	using Type1 = std::decay_t<T1>;
	/*
	������� ��������� �������� �� ������������ �������� ������������ �����
	*/
	void _execute(std::false_type)
	{
		std::invoke(guiFunction, std::move(proxy_value.value));
	}
	/*
	������� ��������� �������� �� ������������ �������� ����� void
	*/
	void _execute(std::true_type)
	{
		std::invoke(guiFunction);
	}
	auto _execute_return(std::false_type)
	{
		return std::invoke(guiFunction, std::move(proxy_value.value));
	}
	/*
	������� ��������� �������� �� ������������ �������� ����� void
	*/
	auto _execute_return(std::true_type)
	{
		return std::invoke(guiFunction);
	}
	template<typename ret_val>
	void _execute_if_return(std::false_type)
	{
		auto tRet = actionAfterEvent();
		if(!tRet->isCommandSet())
		{
			tRet = tRet->afterActionHolder();
		}
		auto valTRer = std::dynamic_pointer_cast<AfterActionHolderValue<ret_val>>(tRet);
		auto retVal = _execute_return(std::is_void<TContainerType>());
		if(valTRer)
		{
			valTRer->setValue(std::move(retVal));
		}
	}
	template<typename ret_val>
	void _execute_if_return(std::true_type)
	{
		_execute(std::is_void<TContainerType>());
	}
	bool exec(std::false_type)
	{
		using ret_val = std::result_of_t<Type1(TContainerType)>;
		_execute_if_return<ret_val>(std::is_void<ret_val>());
		return true;
	}
	bool exec(std::true_type)
	{
		return false;
	}
public:
	QValueGuiSideEvent(T1&& _guiFunction,
					   T2&& errorFunction,
					   const std::shared_ptr<CAfterActionHolderDefault>& actionAfterEvent):
		guiFunction(std::forward<T1>(_guiFunction)), QGuiSideDefaultEvent(std::forward<T2>(errorFunction),
																		  actionAfterEvent)

	{

	}
		bool execute() override
	{
		return exec(std::is_null_pointer<Type1>());
	}

	template<typename TContainerType = TContainerType,
		class = std::enable_if_t<!std::is_void<TContainerType>::value>>
		void setValue(TContainerType&& _value)
	{
		proxy_value.value = std::move(_value);
	}
private:
	utils::TContainerProxy<TContainerType> proxy_value;
	Type1 guiFunction;

};
