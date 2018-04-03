#pragma once

#include <QObject>
#include <QThread>
#include "command_sequence_builder.h"
#include "QExecuteQueryEvent.h"
#include "QExecuteFeatureEvent.h"
#include <QCoreApplication>
#include "get_ret_val.h"
#include "q_db_side_thread_object.h"
class IErrorLogger;
class QDbSideThreadObject;
class QGuiSideThreadObject;
class QDbInteractionThread:public QObject
{
	Q_OBJECT
public:
	QDbInteractionThread(QObject* parent = nullptr);

	QDbInteractionThread(QThread* dbThread,
						 QObject* parent = nullptr);

	virtual ~QDbInteractionThread();

	template<typename Type1 = std::nullptr_t,
		typename Type2 = std::nullptr_t,
		typename Type3 = std::nullptr_t>
		auto query(
			Type1&& query = nullptr,
			Type2&& guiCommand = nullptr,
			Type3&& errorHandler = nullptr,
			int priority = Qt::LowEventPriority)
	{
		using type1 = std::decay_t<Type1>;
		using type2 = std::decay_t<Type2>;
		using ret_val_type = utils::get_ret_val<std::is_null_pointer<type1>::value, type1, void>::type_ret_val;
		using ret_val_type2 = utils::get_ret_val<std::is_null_pointer<type2>::value, type2, ret_val_type>::type_ret_val;
		auto actionHolder = std::make_shared<CAfterActionHolderDefault>();
		actionHolder->setPriority(priority);
		auto needEvent = new QExecuteQueryEvent<Type1, Type2, Type3, ret_val_type>(
			std::forward<Type1>(query), std::forward<Type2>(guiCommand), std::forward<Type3>(errorHandler), actionHolder);
		QCoreApplication::postEvent(dbSideObject.get(), needEvent, priority);
		this->incrementCommandCount();
		return CCommandSequenceBuilder<ret_val_type2>(std::move(actionHolder));
	}
	template<typename Type2>
	auto after_all_query(Type2&& guiCommand)
	{
		return this->query(nullptr, std::forward<Type2>(guiCommand));
	}


	template<typename TFactoryType,
		typename TRequestType = typename TFactoryType::request_ptr,
		typename T2,
		typename T3>
		auto list(TFactoryType& factory, const TRequestType& request, T2&& guiCommand = nullptr,
				  T3&& errorHandler = nullptr,
				  int priority = Qt::LowEventPriority)
	{
		return query([&factory, request]()->auto{return factory.list(request); }, std::forward<T2>(guiCommand), std::forward<T3>(errorHandler), priority);
	}

	template<typename TContainerType,
		typename Type1,
		typename Type2,
		typename Type3 = std::nullptr_t>
		auto db_query(Type1&& query,
					  Type2&& handler,
					  Type3&& errorHandler = nullptr,
					  int priority = Qt::LowEventPriority)
	{
		return this->query(std::forward<Type1>(query), std::forward<Type2>(handler), std::forward<Type3>(errorHandler), priority);

	}


	template<typename T1> auto future_query(T1&& query,
											int priority = Qt::LowEventPriority)
	{
		typedef std::result_of_t<T1()> TContainerType;
		QExecuteFeatureEvent<TContainerType, T1>* newEvent(new QExecuteFeatureEvent<TContainerType, T1>(std::forward<T1>(query)));
		auto returnValue = newEvent->future();
		QCoreApplication::postEvent(dbSideObject.get(), newEvent, priority);
		this->incrementCommandCount();
		return returnValue;
	}
	template<typename Type1>
	auto db_command(Type1&& command, int priority = Qt::LowEventPriority)
	{
		return this->query(std::forward<Type1>(command),
						   nullptr,
						   nullptr,
						   priority);
	}
	template<typename Type1,
		typename Type2,
		typename Type3 = std::nullptr_t>
		auto db_command(Type1&& dbCommand,
						Type2&& guiCommand,
						Type3&& errorHandler = nullptr,
						int priority = Qt::LowEventPriority)
	{
		return this->query(std::forward<Type1>(dbCommand),
						   std::forward<Type2>(guiCommand),
						   std::forward<Type3>(errorHandler), priority);
	}

	QThread* work_hread();
	size_t countTaskInQueue()const;

	void setErrorLogger(IErrorLogger* errorLoger);
	IErrorLogger* errorLoger();

	QDbInteractionThread(const QDbInteractionThread& obj) = delete;
	QDbInteractionThread(QDbInteractionThread&& obj) = delete;
	QDbInteractionThread& operator=(const QDbInteractionThread& obj) = delete;
	QDbInteractionThread& operator=(QDbInteractionThread&& obj) = delete;
signals:
	void stackSizeChange(size_t count);
	void workStart();
	void workEnd();

	private slots:
	void commandExecuted();
	void commandStartExecute();
protected:
	virtual std::unique_ptr<QDbSideThreadObject> createDbSideObject(QGuiSideThreadObject* guiSideObject);
private:
	void incrementCommandCount();
	std::unique_ptr<QDbSideThreadObject> dbSideObject;
	QGuiSideThreadObject* guiSideObject;

	QThread* workThread;

	std::atomic<size_t> countComandsInQueue = 0;


};