/**
*  @file
*  @copyright defined in xmax/LICENSE
*/
#pragma once
#include <unitedb/unitedef.hpp>
#include <boost/interprocess/managed_mapped_file.hpp>
#include <filesystem>

namespace unitedb
{
	namespace fs = std::filesystem;
	namespace inpr = boost::interprocess;
	using mapped_file = inpr::managed_mapped_file;
	using segment_manager = inpr::managed_mapped_file::segment_manager;



	template<typename T>
	using DBAlloc = inpr::allocator<T, segment_manager>;


	using DefAlloc = DBAlloc<char>;

	using MString = std::basic_string< char, std::char_traits< char >, DBAlloc< char > >; // mapped string.

	template<typename T>
	using MVector = std::vector<T, DBAlloc<T> >; // mapped vector.

	constexpr DBRevision InvalidRevision = -1;

	class IDBTable
	{
	public:
		virtual ~IDBTable() {}
	};



}

#define DB_ASSERT(expr) BOOST_ASSERT(expr)

#define DB_ASSERT_MSG(expr, msg) BOOST_ASSERT_MSG(expr, msg)

#define DB_THROW(expr) BOOST_THROW_EXCEPTION(expr)