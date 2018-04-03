#pragma once

#include "QDbEvent.h"
#include <functional> 
#include <memory>
#include "QValueGuiSideEvent.h"
#include "after_action_holder_interface.h"

template<typename T1, typename T2, typename T3, typename TContType> class QExecuteQueryEvent:public QDbEvent
{
	//constexpr static auto EventType = QEvent::Type::User + 10;
	using Type1 = std::decay_t<T1>;
	using Type2 = std::decay_t<T2>;
	using Type3 = std::decay_t<T3>;

	void check_void(std::true_type)
	{
		std::invoke(dbSideCommand);
	}
	void check_void(std::false_type)
	{
		guiEventObj->setValue(std::invoke(dbSideCommand));
	}
	void check_is_function(std::true_type)
	{
		if(dbSideCommand)
		{
			check_void(std::is_void<TContType>());
		}
	}
	void check_is_function(std::false_type)
	{
		check_void(std::is_void<TContType>());
	}
	void exec(std::false_type)
	{
		check_is_function(std::is_same<Type1, std::function<TContType()>>());
	}
	void exec(std::true_type)
	{
	}
public:

	QExecuteQueryEvent(T1&& _dbSideCommand,
					   T2&& _guiCommand,
					   T3&& _errorHandler,
					   std::shared_ptr<CAfterActionHolderDefault> _afterActionHolder
					   ) :QDbEvent(),
						  dbSideCommand(std::forward<T1>(_dbSideCommand)),
						  guiCommand(std::forward<T2>(_guiCommand)),
						  errorHandler(std::forward<T3>(_errorHandler)),
						  afterActionHolder(std::move(_afterActionHolder))
	{
	}

	virtual ~QExecuteQueryEvent() = default;

	virtual void executeDbPart() override
	{
		guiEventObj = std::make_unique<QValueGuiSideEvent<Type2, Type3, TContType>>(std::move(guiCommand),
						  std::move(errorHandler), std::move(afterActionHolder));
		try
		{
			exec(std::is_null_pointer<Type1>());
		}
		catch (...)
		{
			guiEventObj->setExceptionPtr(std::current_exception());
		}
	}

	virtual QGuiSideEvent* guiEvent() override
	{
		return guiEventObj.release();
	}

private:
	Type1 dbSideCommand;
	Type2 guiCommand;
	std::unique_ptr<QValueGuiSideEvent<Type2, Type3, TContType>> guiEventObj;
	Type3 errorHandler;
	std::exception_ptr exception;
	std::shared_ptr<CAfterActionHolderDefault> afterActionHolder;
};
