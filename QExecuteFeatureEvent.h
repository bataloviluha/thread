#pragma once

#include "QDbEvent.h"
#include <future>
#include <functional>
#include "QGuiSideNoActionEvent.h"

template<typename TFeatureClass,typename TFunctionType> class QExecuteFeatureEvent :public QDbEvent
{
	public:
		
		QExecuteFeatureEvent(TFunctionType&& functor):QDbEvent(),
			eventFunctor(std::forward<TFunctionType>(functor))
		{
		}

		virtual void executeDbPart() override
		{
			try
			{
#ifdef _DEBUG
				auto handle = QThread::currentThreadId();
#endif
				eventPromise.set_value(eventFunctor());
			}
			catch (...)
			{
				eventPromise.set_exception(std::current_exception());
			}
		}

		std::future<TFeatureClass> future()
		{
			return eventPromise.get_future();
		}

		virtual QGuiSideEvent* guiEvent() override
		{
			//eventGui = new QGuiSideEvent();
			if (!eventGui)
			{
				return new QGuiSideNoActionEvent();
				//eventGui = std::make_unique<QGuiSideNoActionEvent>();
			}
			return eventGui.release();
		}
	private:
		std::promise<TFeatureClass> eventPromise;
		TFunctionType eventFunctor;

		std::unique_ptr<QGuiSideEvent> eventGui;
};
