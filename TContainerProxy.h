#pragma once
namespace utils
{
	template<typename TContainerType>
	class TContainerProxy
	{
	public:
		TContainerType value;
	};
	template<>
	class TContainerProxy<void>
	{
	};
}