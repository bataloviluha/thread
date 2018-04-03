#include "q_db_interaction_thread.h"
#include "QCoreApplication"
#include <QDebug>
#include "q_gui_side_thread_object.h"

QDbInteractionThread::QDbInteractionThread(QObject* parent):QObject(parent), countComandsInQueue(0)
{
	workThread = new QThread(this);
	guiSideObject = new QGuiSideThreadObject(this);
	//dbSideObject = new QDbSideThreadObject(guiSideObject);
	dbSideObject = createDbSideObject(guiSideObject);
	connect(guiSideObject, &QGuiSideThreadObject::eventExecuted, this, &QDbInteractionThread::commandExecuted);
	workThread->start();
	dbSideObject->moveToThread(workThread);
#ifdef _DEBUG
	qDebug() << this << " create witch owner thread";
#endif
}

QDbInteractionThread::QDbInteractionThread(QThread* _dbThread,
										   QObject* parent) :QObject(parent), workThread(_dbThread), countComandsInQueue(0)
{
	guiSideObject = new QGuiSideThreadObject(this);
	//dbSideObject = new QDbSideThreadObject(guiSideObject);
	dbSideObject = createDbSideObject(guiSideObject);
	connect(guiSideObject, &QGuiSideThreadObject::eventExecuted, this, &QDbInteractionThread::commandExecuted);
	workThread->start();
	dbSideObject->moveToThread(workThread);
#ifdef _DEBUG
	qDebug() << this << " create witch thread";
#endif
}

QDbInteractionThread::~QDbInteractionThread()
{
#ifdef _DEBUG
	qDebug() << this << " db thread destructor called";
#endif

	if (workThread->parent() == this)
	{
		workThread->quit();
		workThread->wait();
	}
	//delete dbSideObject;
}

void QDbInteractionThread::incrementCommandCount()
{
	//QMetaObject::invokeMethod(this, "commandStartExecute", Qt::QueuedConnection);
	countComandsInQueue++;
#ifdef _DEBUG
	qDebug() << this << " increment commands count " << countComandsInQueue;
#endif

	if (countComandsInQueue == 1)
	{
#ifdef _DEBUG
		qDebug() << this << " work start called ";
#endif

		emit workStart();
	}

	emit stackSizeChange(countComandsInQueue);
}

void QDbInteractionThread::commandExecuted()
{
	countComandsInQueue--;
#ifdef _DEBUG
	qDebug() << this << " decrement commands count " << countComandsInQueue;
#endif

	emit stackSizeChange(countComandsInQueue);
	if (countComandsInQueue==0)
	{
#ifdef _DEBUG
		qDebug() << this << " work end called ";
#endif

		emit workEnd();
	}
}

void QDbInteractionThread::commandStartExecute()
{

	countComandsInQueue++;
#ifdef _DEBUG
	qDebug() << this << " increment commands count " << countComandsInQueue;
#endif

	if (countComandsInQueue == 1)
	{
#ifdef _DEBUG
		qDebug() << this << " work start called ";
#endif

		emit workStart();
	}

	emit stackSizeChange(countComandsInQueue);

}

std::unique_ptr<QDbSideThreadObject> QDbInteractionThread::createDbSideObject(QGuiSideThreadObject* guiSideObject)
{
	return std::make_unique<QDbSideThreadObject>(guiSideObject);
}

//CCommandSequenceBuilder QDbInteractionThread::db_command(std::function<void()> command, int priority)
//{
//	std::shared_ptr<CAfterActionHolderDefault> actionHolder(new CAfterActionHolderDefault());
//	actionHolder->setPriority(priority);
//	
//	QExecuteCommandEvent* needEvent(new QExecuteCommandEvent(command, std::function<void()>(), std::function<void()>(), actionHolder));
//	QCoreApplication::postEvent(dbSideObject, needEvent, priority);
//	this->incrementCommandCount();
//	return CCommandSequenceBuilder(actionHolder);
//}
//
//CCommandSequenceBuilder QDbInteractionThread::db_command(std::function<void()> dbCommand,
//	                                                     std::function<void()> guiCommand,
//	                                                     std::function<void()> errorHandler, 
//	                                                     int priority)
//{
//	std::shared_ptr<CAfterActionHolderDefault> actionHolder(new CAfterActionHolderDefault());
//	actionHolder->setPriority(priority);
//
//	QExecuteCommandEvent* needEvent(new QExecuteCommandEvent(dbCommand, guiCommand, errorHandler, actionHolder));
//	QCoreApplication::postEvent(dbSideObject, needEvent, priority);
//	this->incrementCommandCount();
//	return CCommandSequenceBuilder(actionHolder);
//
//}

QThread* QDbInteractionThread::work_hread()
{
	return workThread;
}
size_t QDbInteractionThread::countTaskInQueue()const
{
	return countComandsInQueue;
}

void QDbInteractionThread::setErrorLogger(IErrorLogger* errorLoger)
{
	guiSideObject->setErrorLogger(errorLoger);
}

IErrorLogger* QDbInteractionThread::errorLoger()
{
	return guiSideObject->errorLogger();
}