#pragma once

#include "q_db_interaction_thread.h"
#include "otl_wrapper/src/data_stream.h"
#include "factory_container/transaction_controller_interface.h"

class OTLConnectionProvider;

class QDbInteractionThreadWitchConnection:public QDbInteractionThread
{
	Q_OBJECT
	public:
		QDbInteractionThreadWitchConnection(
			OTLConnectionProvider& provider,
			QThread* dbThread,
			QObject* parent = nullptr);
	protected:
		virtual std::unique_ptr<QDbSideThreadObject> createDbSideObject(QGuiSideThreadObject* guiSideObject);
	signals:
		void session_lost();//������ ������� �������������, ����� ������ �������� 
		void session_try_recconect(int tryNum);//������ ������� �������������, ����� ��������������� ������� ����������� � ����� ������  
		void session_reconnected();//������ ������� ������������� 
		void session_try_fail(int);
		void session_recconect_fail();
		void session_try_reconnect();

	private:
		OTLConnectionProvider& provider; 
};

