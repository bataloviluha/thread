#pragma once

#include "after_action_holder_interface.h"
#include "qnamespace.h"
#include "TContainerProxy.h"
class QDbInteractionThread;
class CAfterActionHolderDefault:public std::enable_shared_from_this<CAfterActionHolderDefault>//:public IAfterActionHolder
{
public:
	CAfterActionHolderDefault() = default;
	virtual ~CAfterActionHolderDefault() = default;
	void setAfterActionHolder(const std::shared_ptr<CAfterActionHolderDefault>& afterActionHolder)/*override*/;

	const std::shared_ptr<CAfterActionHolderDefault>& afterActionHolder()/*override*/;

	virtual bool isCommandSet() const;

	virtual void putAfterCommand(QDbInteractionThread* thread)/*override*/;

	int commandPriority()const /*override*/;

	void setPriority(int priority);
private:
	std::shared_ptr<CAfterActionHolderDefault> tail = nullptr;
	int priority = Qt::LowEventPriority;
};
template<typename TRetType>
class AfterActionHolderValue:
	public CAfterActionHolderDefault
{
protected:
	utils::TContainerProxy<TRetType> proxy_value;
public:
	AfterActionHolderValue() = default;
	virtual ~AfterActionHolderValue() = default;

	template<typename TRetType = TRetType,
		class = std::enable_if_t<!std::is_void<TRetType>::value>>
		void setValue(TRetType&& _value)
	{
		proxy_value.value = std::move(_value);
	}
};