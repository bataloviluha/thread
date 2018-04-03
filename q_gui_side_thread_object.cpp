#include "q_gui_side_thread_object.h"
#include "QGuiSideEvent.h"
#include "q_db_interaction_thread.h"
//#include "error_handle.h"
#include "utils\error_handle.h"

QGuiSideThreadObject::QGuiSideThreadObject(QDbInteractionThread* parent):QObject(parent), parentThread(parent)
{
}

bool QGuiSideThreadObject::event(QEvent* event)
{
	if (event->type()== QGuiSideEvent::EventType)
	{
		QGuiSideEvent* guiEvent = static_cast<QGuiSideEvent*>(event);
		if (!guiEvent->exceptionInfo())
		{
			guiEvent->execute();
			if (guiEvent->actionAfterEvent())
			{
				guiEvent->actionAfterEvent()->putAfterCommand(parentThread);
				/*if (guiEvent->actuionAfterEvent()->isCommandSet())
				{
					guiEvent->actuionAfterEvent()->putAfterCommand(parentThread);
				}*/
				/*else if (guiEvent->actuionAfterEvent()->afterActionHolder())
				{
					guiEvent->actuionAfterEvent()->afterActionHolder()->putAfterCommand(parentThread);
				}*/
			}
		}
		else
		{
			//guiEvent->executeError();
			handle_code([guiEvent = guiEvent]
			{
				std::rethrow_exception(guiEvent->exceptionInfo());
			}, error_loger);
		}
		emit eventExecuted();
		return true;
	}
	return QObject::event(event);
}


void QGuiSideThreadObject::setErrorLogger(IErrorLogger* errorLoger)
{
	error_loger = errorLoger;
}

IErrorLogger* QGuiSideThreadObject::errorLogger()
{
	return error_loger;
}
