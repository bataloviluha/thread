#pragma once
#include <type_traits>
namespace utils
{
	template<bool _Test, typename Type1, typename InputVal>
	struct get_ret_val
	{
		using type_ret_val = typename std::result_of_t<Type1(InputVal)>;
	};
	template<typename Type1, typename InputVal>
	struct get_ret_val<true, Type1, InputVal>
	{
		using type_ret_val = void;
	};
}