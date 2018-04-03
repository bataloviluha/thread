#pragma once

#include <QObject>

class QDbInteractionThread;
class IErrorLogger;

class QGuiSideThreadObject :public QObject
{
	Q_OBJECT
	public:
		QGuiSideThreadObject(QDbInteractionThread* parent);
		bool event(QEvent* event);

		void setErrorLogger(IErrorLogger* errorLoger);
		IErrorLogger* errorLogger();

	signals:
		void eventExecuted();
	private:
		QDbInteractionThread* parentThread=nullptr;
		IErrorLogger* error_loger;
};
