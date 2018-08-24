/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <chaintypes.hpp>
#include <unitedb/dbtable.hpp>
namespace chain
{
	namespace db = unitedb;

	namespace indexed = db::multi_index;

	template<typename _Object, typename... _Args>
	using IndexedBy = db::DBIndexedBy<_Object, _Args...>;

	template<typename _multi_index>
	using Table = db::DBTable<_multi_index>;

	class ObjectType
	{
	public:
		enum Code
		{
			NullObject = 0,
			AccountObject,
		};
	};
}