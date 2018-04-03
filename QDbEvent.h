#pragma once

#include <QEvent>
#include "QGuiSideEvent.h"
#include <exception>
#include "after_action_holder_interface.h"

class QDbEvent:public QEvent
{
public:
	constexpr static auto EventType = QEvent::Type::User + 10;

		QDbEvent();
		virtual ~QDbEvent() = default;
		virtual void executeDbPart()abstract;
		virtual QGuiSideEvent* guiEvent()abstract;
};