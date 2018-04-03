#pragma once

#include "q_db_side_thread_object.h"

class OTLConnectionProvider;

class QDbSideThreadObjectWithConnection:public QDbSideThreadObject
{
	Q_OBJECT
	public:
		QDbSideThreadObjectWithConnection(OTLConnectionProvider& connectionProvaider, QGuiSideThreadObject*, QObject* parent = nullptr);
		bool event(QEvent*)override;
	signals:
		void session_lost(const std::string& reason);
		void session_try_reconnect(int numTry);
		void session_try_fail(int numTry);
		void session_reconnected();
		void session_reconnect_fail();
	private:
		OTLConnectionProvider& connectionProvaider;
		int maxCountTry = 6;
};