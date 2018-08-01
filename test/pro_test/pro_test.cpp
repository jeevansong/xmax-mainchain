#define BOOST_TEST_MODULE pro_test

#include <boost/test/included/unit_test.hpp>
#include <pro/utils/singleton.hpp>
#include <pro/types/time.hpp>
#include <pro/types/any_value.hpp>
#include <pro/types/any_object.hpp>
#include <pro/utils/string_utils.hpp>
#include <pro/scode/shortname.hpp>
#include <pro/types/tree.hpp>

using namespace pro;

BOOST_AUTO_TEST_SUITE(pro_utils_suite)

namespace {
	class TestSingleton : public pro::Singleton<TestSingleton> {
		DECLARE_USE_SINGLETON(TestSingleton);

	public:
		void SetA(int a) {
			a_ = a;
		}

		int GetA() const { return a_; }

	private:
		int a_{ 0 };
	};

	TestSingleton::TestSingleton() {}
	TestSingleton::~TestSingleton() {}
}

BOOST_AUTO_TEST_CASE(test_singleton) {
	TestSingleton& s1 = TestSingleton::GetInstance();
	TestSingleton::GetInstance().SetA(12345);
	BOOST_CHECK(s1.GetA() == 12345);
}

BOOST_AUTO_TEST_CASE(pro_time_1)
{
	TimeMilliseconds milli(2500ll);
	TimeMicroseconds micro = milli.ToTime<TimeMicroseconds>();
	TimeMilliseconds milli2 = micro.ToTime<TimeMilliseconds>();

	TimeSeconds sec = micro.ToTime<TimeSeconds>();

	TimeMicroseconds micro2 = sec.ToTime<TimeMicroseconds>();
	TimeMilliseconds milli3 = sec.ToTime<TimeMilliseconds>();

	BOOST_CHECK(milli.GetValue() == 2500ll);
	BOOST_CHECK(micro.GetValue() == 2500ll * 1000ll);
	BOOST_CHECK(milli2.GetValue() == 2500ll);

	BOOST_CHECK(sec.GetValue() == 2ll);
	BOOST_CHECK(micro2.GetValue() == 2ll * 1000ll * 1000ll);
	BOOST_CHECK(milli3.GetValue() == 2ll * 1000ll);
}

BOOST_AUTO_TEST_CASE(any_value)
{
	const char tstring[] = "test string";
	pro::AnyValue anyi32 = (int32_t)53;
	pro::AnyValue anyi64 = (int64_t)64;
	pro::AnyValue anystr = tstring;
	pro::AnyValue anystr2 = anystr;
	int32_t c = anyi32.CastTo<int32_t>(); 
	BOOST_CHECK(c == 53);
	std::string ss = anystr2.CastTo<string>();

	BOOST_CHECK(ss == tstring);

	pro::AnyValue bValue = true;
	bool b = bValue.CastTo<bool>();
	BOOST_CHECK(b == true);
}

bool checkformat(const string& checkstring, const string& fmt, const AnyObject& args)
{
	string fstring = utils::StringFormat(fmt, args);

	return (fstring == checkstring);
}


BOOST_AUTO_TEST_CASE(string_format)
{
	BOOST_CHECK(checkformat("hello", "hello", AnyObject()));

	BOOST_CHECK(checkformat("hello lily", "hello ${name}", PACK_ARGS( ("name", "lily") )));
}

bool checkname(string real, pro::ShortName name)
{
	string sname = name.ToString();

	return sname == real;
}

BOOST_AUTO_TEST_CASE(scode)
{
	BOOST_CHECK(checkname("xmax", SN(xmax)));
	BOOST_CHECK(checkname("567890@", SN(567890@)));
	BOOST_CHECK(checkname("m190@xm.c", SN(m190@xm.c)));

	// check over name.
	BOOST_CHECK(checkname("m19adfgge15", SN(m19adfgge15640@xm1345678275.c)));


	// check LegalName
	BOOST_CHECK(ShortName::LegalName("xmax") == true);
	BOOST_CHECK(ShortName::LegalName("567890@") == true);
	BOOST_CHECK(ShortName::LegalName("xmax$%") == false);

	BOOST_CHECK(ShortName::LegalName("m19adfgge15640@xm1345678275.c") == false);

	// check GlyphBitLength
	for (int i = 1; i < SCODE_REGION_0_SIZE; ++i)
	{
		BOOST_CHECK(ShortName::GlyphBitLength(SCODE_REGION_0[i]) == SCODE_R_0_BITS);
	}
	for (int i = 1; i < SCODE_REGION_1_SIZE; ++i)
	{
		BOOST_CHECK(ShortName::GlyphBitLength(SCODE_REGION_1[i]) == SCODE_R_1_BITS);
	}
	BOOST_CHECK(ShortName::GlyphBitLength('#') == 0);
	BOOST_CHECK(ShortName::GlyphBitLength('(') == 0);

	BOOST_CHECK(ShortName::NameLengthWithBit("xmax") == (SCODE_R_0_BITS * 4));
	BOOST_CHECK(ShortName::NameLengthWithBit("124.@") == (SCODE_R_0_BITS * 4 + SCODE_R_1_BITS * 1));
	BOOST_CHECK(ShortName::NameLengthWithBit("1.@mx") == (SCODE_R_0_BITS * 4 + SCODE_R_1_BITS * 1));
	BOOST_CHECK(ShortName::NameLengthWithBit("1.###@mx") == (SCODE_R_0_BITS * 4 + SCODE_R_1_BITS * 1));
}


BOOST_AUTO_TEST_CASE(test_boost_tree) {

	pro::Tree<std::string> root("rt");
	pro::Tree<std::string> c1("c1");
	pro::Tree<std::string> c2("c2");
	pro::Tree<std::string> c3("c3");
	pro::Tree<std::string> c4("c4");
	pro::Tree<std::string> c5("c5");
	pro::Tree<std::string> c6("c6");
	pro::Tree<std::string> c7("c7");
	pro::Tree<std::string> c8("c8");


	c5.AddChild(c7);
	c5.AddChild(c8);
	c2.AddChild(c5);
	c2.AddChild(c6);

	c1.AddChild(c3);
	c1.AddChild(c4);

	root.AddChild(c1);
	root.AddChild(c2);
	
	

	std::string output;
	root.Traverse([&output](pro::Tree<std::string>* tn) {
		output += tn->value() + " ";
	});

	BOOST_CHECK(output.compare("c3 c4 c1 c7 c8 c5 c6 c2 rt ") == 0);
}


BOOST_AUTO_TEST_SUITE_END()