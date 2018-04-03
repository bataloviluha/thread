#include "q_db_side_thread_object_with_connection.h"
#include "otl_wrapper\src\otl_config.h"
#include "QT_ex\OTLConnectionProvider\OTLConnectionProvider.h"

QDbSideThreadObjectWithConnection::QDbSideThreadObjectWithConnection(OTLConnectionProvider& _connectionProvaider, QGuiSideThreadObject* guiObj, QObject* parent):QDbSideThreadObject(guiObj, parent	), connectionProvaider(_connectionProvaider)
{

}

bool QDbSideThreadObjectWithConnection::event(QEvent* e)
{
	//bool canExecute = true;
	
	while (true)
	{
		int reconnectTry = 1;
		try
		{
			return QDbSideThreadObject::event(e);
			
		}
		catch (otl_exception& ex)
		{
			if (ex.code == 12571|| ex.code== 3135|| ex.code == 3114 || ex.code== 12571)
			{
				bool isReconnected = false;
				emit session_lost("session connection lost");
				while (reconnectTry <= maxCountTry)
				{
					if (connectionProvaider.connect(connectionProvaider.user(), connectionProvaider.password(), connectionProvaider.db()))
					{
						emit session_reconnected();
						isReconnected = true;
						break;
					}
					else
					{
						emit session_try_fail(reconnectTry);
					}
					reconnectTry++;
				}
				if (!isReconnected)
				{
					session_reconnect_fail();
					throw ex;
				}
			}
			else
			{
				throw ex;
			}
		}
	}
}

