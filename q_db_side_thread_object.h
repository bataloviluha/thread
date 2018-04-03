#pragma once

#include <QObject>

class QGuiSideThreadObject;
class QDbSideThreadObject:public QObject
{
	Q_OBJECT
	public:
		QDbSideThreadObject(QGuiSideThreadObject*, QObject* parent=nullptr);
		~QDbSideThreadObject();

		bool event(QEvent*)override;
	protected:
		QGuiSideThreadObject* get_reciver();
	private:
		QGuiSideThreadObject* reciver;
};