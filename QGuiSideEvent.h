#pragma once

#include <QEvent> 
#include <exception>
#include <memory>
#include "after_action_holder_default.h"

class QGuiSideEvent:public QEvent
{
	public:
		static const int EventType = QEvent::User + 11;

		QGuiSideEvent();
		virtual ~QGuiSideEvent() = default;

		virtual bool execute()abstract;
		virtual bool executeError()abstract;
		virtual std::exception_ptr exceptionInfo()abstract;
		virtual std::shared_ptr<CAfterActionHolderDefault> actionAfterEvent()abstract;
};

