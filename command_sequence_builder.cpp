#include "command_sequence_builder.h"

CCommandSequenceBuilder::CCommandSequenceBuilder(std::shared_ptr<IAfterActionHolder>&& _tail):
	tail(std::move(_tail))
{
}

//CCommandSequenceBuilder& CCommandSequenceBuilder::then_command(std::function<void()> query,
//	std::function<void()> handler,
//	std::function<void()> errorHandler )
//{
//	auto newCommand = std::make_shared<CAfterActionHolderCommand>(query, handler, errorHandler);
//	if (tail)
//	{
//		tail->setAfterActionHolder(newCommand);
//	}
//	tail = newCommand;
//	return *this;
//}

CCommandSequenceBuilder::CCommandSequenceBuilder(CCommandSequenceBuilder&& obj)
{
	tail = std::move(obj.tail);
}

void CCommandSequenceBuilder::setTailFrom(std::shared_ptr<IAfterActionHolder> sequenceHead)
{
	if (tail)
	{
		tail->setAfterActionHolder(sequenceHead);
	}
	auto current = sequenceHead->afterActionHolder();
	while (current)
	{
		if (!current->afterActionHolder())
		{
			tail = current;
		}
		current = current->afterActionHolder();
	}
}

