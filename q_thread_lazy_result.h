#pragma once


#include <QObject>
#include "q_db_interaction_thread.h"

class QDbThreadLazyResultBase:public QObject
{
	Q_OBJECT
	public:
		QDbThreadLazyResultBase()
		{
			connect(this, &QDbThreadLazyResultBase::on_data_resived, this, &QDbThreadLazyResultBase::data_resived, Qt::QueuedConnection);
		}
		virtual ~QDbThreadLazyResultBase()=default;
	signals:
		void on_data_resived();
	private slots:
		void data_resived()
		{
			data_resived_impl();
		}
	protected:
		void disconnect_signal()
		{
			disconnect(this, &QDbThreadLazyResultBase::on_data_resived, this, &QDbThreadLazyResultBase::data_resived);
		}
	private:
		virtual void data_resived_impl()abstract;
};

template<typename TResultType> class TDbThreadLazyResultInterface :public QDbThreadLazyResultBase
{
	public:
		virtual std::shared_future<TResultType> result_as_future()abstract;
};

template<typename TResultType, typename THanleFunction> class TDbThreadLazyResult:public TDbThreadLazyResultInterface<TResultType>
{
	public:
		template<typename TExecFunct>
			TDbThreadLazyResult(QDbInteractionThread& thread,
						  TExecFunct&& dbFunction,
						  THanleFunction&& guiFunction):handler(std::forward<THanleFunction>(guiFunction))
		{
			promice = std::make_shared<std::promise<TResultType>>();
			resultBuffer = promice->get_future();
			destroed = std::make_shared<bool>(false);

			thread.query([promicePtr = promice, thisProxy = this, dbFunction= std::forward<TExecFunct>(dbFunction), destroed= destroed]()
			{
				try {

					promicePtr->set_value(dbFunction());
				}
				catch (...)
				{
					promicePtr->set_exception(std::current_exception());
				}
				if (!*destroed)
				{
					emit thisProxy->on_data_resived();
				}

			});
		}

		~TDbThreadLazyResult()
		{
			disconnect_signal();
			*destroed = true;
		}

		std::shared_future<TResultType> result_as_future()override
		{
			disconnect_signal();
			return resultBuffer;
		}

	private:
		void data_resived_impl()
		{
			auto temp = std::move(resultBuffer);
			handler(temp.get());
		}
		std::shared_ptr<std::promise<TResultType> > promice;
		std::shared_future<TResultType> resultBuffer;
		THanleFunction handler;
		std::shared_ptr<bool> destroed;
};

template<typename TResultType, 
	     typename TExecFunct, 
	     typename THanleFunction> auto make_lazy_result(QDbInteractionThread& thread,
																	         TExecFunct&& funct,
																	         THanleFunction&& guiFunction)
{
	return std::make_shared<TDbThreadLazyResult<TResultType, THanleFunction>>(thread,std::forward<TExecFunct>(funct), std::forward<THanleFunction>(guiFunction));
}