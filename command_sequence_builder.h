#pragma once
#include "after_action_holder_select.h"
#include <memory>
#include "utils\traits.h"
#include <QThread>
#include "QValueGuiSideEvent.h"
#include "get_ret_val.h"
template<typename Type>
class CCommandSequenceBuilder
{
public:
	CCommandSequenceBuilder(std::shared_ptr<CAfterActionHolderDefault>&& _tail) noexcept :
		tail(std::move(_tail))
	{

	}
	CCommandSequenceBuilder(const CCommandSequenceBuilder&) = delete;
	CCommandSequenceBuilder& operator=(const CCommandSequenceBuilder&) = delete;
	CCommandSequenceBuilder(CCommandSequenceBuilder&&) = default;
	CCommandSequenceBuilder& operator=(CCommandSequenceBuilder&&) = delete;

	template<typename T1 = std::nullptr_t,
		typename T2 = std::nullptr_t,
		typename T3 = std::nullptr_t
	>
		auto then_query(T1&& query = nullptr,
											T2&& handler = nullptr,
											T3&& errorHandler = nullptr,
											int priority = Qt::LowEventPriority)
	{
		using ret_val_type = utils::get_ret_val<std::is_null_pointer<T1>::value, T1, Type>::type_ret_val;
		using ret_val_type2 = utils::get_ret_val<std::is_null_pointer<T2>::value, T2, ret_val_type>::type_ret_val;
		auto selectCommandHandler = std::make_shared<TAfterActionHolderSelect<T1, T2, T3, Type>>(std::forward<T1>(query),
																								 std::forward<T2>(handler),
																								 std::forward<T3>(errorHandler));
		if(tail)
		{
			tail->setAfterActionHolder(selectCommandHandler);
		}
		else
		{
			tail = selectCommandHandler;
		}
		return CCommandSequenceBuilder<ret_val_type2>(std::move(tail));;
	}



	void setTailFrom(std::shared_ptr<CAfterActionHolderDefault> sequenceHead)
	{
		if(tail)
		{
			tail->setAfterActionHolder(sequenceHead);
		}
	}

private:
	std::shared_ptr<CAfterActionHolderDefault> tail;
};
