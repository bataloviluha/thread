#include "after_action_holder_default.h"

void CAfterActionHolderDefault::setAfterActionHolder(const std::shared_ptr<CAfterActionHolderDefault>& afterActionHolder)
{
	if(tail)
	{
		tail->setAfterActionHolder(afterActionHolder);
	}
	else
	{
		tail = afterActionHolder;
	}
}

const std::shared_ptr<CAfterActionHolderDefault>& CAfterActionHolderDefault::afterActionHolder()
{
	return tail;
}

bool CAfterActionHolderDefault::isCommandSet() const
{
	return false;
}

void CAfterActionHolderDefault::putAfterCommand(QDbInteractionThread* dbThread)
{
	if(tail)
	{
		tail->putAfterCommand(dbThread);
	}
}

int CAfterActionHolderDefault::commandPriority() const
{
	return priority;
}

void CAfterActionHolderDefault::setPriority(int _priority)
{
	priority = _priority;
}
