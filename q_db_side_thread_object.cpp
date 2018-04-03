#include "q_db_side_thread_object.h"
#include "QDbEvent.h"
#include <QCoreApplication>
#include "q_gui_side_thread_object.h"

QDbSideThreadObject::QDbSideThreadObject(QGuiSideThreadObject* guiSideObject,QObject* parent):reciver(guiSideObject)
{
}

QDbSideThreadObject::~QDbSideThreadObject()
{
}

QGuiSideThreadObject* QDbSideThreadObject::get_reciver()
{
	return reciver;
}

bool QDbSideThreadObject::event(QEvent* event)
{
	if (event->type() == QDbEvent::EventType)
	{
		QDbEvent* dbEvent = static_cast<QDbEvent*>(event);
		dbEvent->executeDbPart();
		auto guiEvent = dbEvent->guiEvent();
		if (guiEvent)
		{
			QCoreApplication::postEvent(reciver, guiEvent);
		}
	}
	return QObject::event(event);
}
