#include "q_db_iteraction_thread_with_connection.h"
#include "q_db_side_thread_object_with_connection.h"


QDbInteractionThreadWitchConnection::QDbInteractionThreadWitchConnection(
	OTLConnectionProvider& _provider,
	QThread* dbThread,
	QObject* parent):QDbInteractionThread(dbThread, parent), provider(_provider)
{
}

std::unique_ptr<QDbSideThreadObject> QDbInteractionThreadWitchConnection::createDbSideObject(QGuiSideThreadObject* guiSideObject)
{
	auto result= std::make_unique<QDbSideThreadObjectWithConnection>(provider, guiSideObject);
	connect(&*result,&QDbSideThreadObjectWithConnection::session_lost,this, &QDbInteractionThreadWitchConnection::session_lost,Qt::QueuedConnection);
	connect(&*result, &QDbSideThreadObjectWithConnection::session_reconnected, this, &QDbInteractionThreadWitchConnection::session_reconnected, Qt::QueuedConnection);
	connect(&*result, &QDbSideThreadObjectWithConnection::session_try_fail, this, &QDbInteractionThreadWitchConnection::session_try_fail, Qt::QueuedConnection);
	connect(&*result, &QDbSideThreadObjectWithConnection::session_try_reconnect, this, &QDbInteractionThreadWitchConnection::session_try_reconnect, Qt::QueuedConnection);
	connect(&*result, &QDbSideThreadObjectWithConnection::session_reconnect_fail, this, &QDbInteractionThreadWitchConnection::session_recconect_fail, Qt::QueuedConnection);
	return std::move(result);
}
