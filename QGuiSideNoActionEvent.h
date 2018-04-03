#pragma once

#include "QGuiSideDefaultEvent.h"
#include <functional>

class QGuiSideNoActionEvent :public QGuiSideDefaultEvent<std::nullptr_t>
{
	public:
		QGuiSideNoActionEvent() = default;
		virtual ~QGuiSideNoActionEvent() = default;
		bool execute() override
		{
			return true;
		}

};
