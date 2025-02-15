#define BOOST_TEST_MODULE dep0_llvmgen_tests_0001_integral_primitives
#include <boost/test/included/unit_test.hpp>

#include "llvmgen_tests_fixture.hpp"

BOOST_FIXTURE_TEST_SUITE(dep0_llvmgen_tests_0001_integral_primitives, LLVMGenTestsFixture)

BOOST_AUTO_TEST_CASE(pass_000)
{
    BOOST_TEST_REQUIRE(pass("0001_integral_primitives/pass_000.depc"));
    BOOST_TEST(has_function("f0", [] (llvm::Function const& f)
    {
        BOOST_TEST(f.hasAttribute(llvm::AttributeList::ReturnIndex, llvm::Attribute::SExt));
        BOOST_TEST(is_return_of(f.getEntryBlock().getTerminator(), signed_constant(127)));
        return boost::test_tools::predicate_result(true);
    }));
    BOOST_TEST(has_function("f18", [] (llvm::Function const& f)
    {
        BOOST_TEST(f.hasAttribute(llvm::AttributeList::ReturnIndex, llvm::Attribute::SExt));
        BOOST_TEST(is_return_of(f.getEntryBlock().getTerminator(), signed_constant(32767)));
        return boost::test_tools::predicate_result(true);
    }));
    BOOST_TEST(has_function("f42", [] (llvm::Function const& f)
    {
        BOOST_TEST(f.hasAttribute(llvm::AttributeList::ReturnIndex, llvm::Attribute::SExt));
        BOOST_TEST(is_return_of(f.getEntryBlock().getTerminator(), signed_constant(2147483647)));
        return boost::test_tools::predicate_result(true);
    }));
    BOOST_TEST(has_function("f81", [] (llvm::Function const& f)
    {
        BOOST_TEST(f.hasAttribute(llvm::AttributeList::ReturnIndex, llvm::Attribute::SExt));
        BOOST_TEST(is_return_of(f.getEntryBlock().getTerminator(), signed_constant(9223372036854775807)));
        return boost::test_tools::predicate_result(true);
    }));
    BOOST_TEST(has_function("f141", [] (llvm::Function const& f)
    {
        BOOST_TEST(f.hasAttribute(llvm::AttributeList::ReturnIndex, llvm::Attribute::ZExt));
        BOOST_TEST(is_return_of(f.getEntryBlock().getTerminator(), unsigned_constant(255ul)));
        return boost::test_tools::predicate_result(true);
    }));
    BOOST_TEST(has_function("f153", [] (llvm::Function const& f)
    {
        BOOST_TEST(f.hasAttribute(llvm::AttributeList::ReturnIndex, llvm::Attribute::ZExt));
        BOOST_TEST(is_return_of(f.getEntryBlock().getTerminator(), unsigned_constant(65535ul)));
        return boost::test_tools::predicate_result(true);
    }));
    BOOST_TEST(has_function("f169", [] (llvm::Function const& f)
    {
        BOOST_TEST(f.hasAttribute(llvm::AttributeList::ReturnIndex, llvm::Attribute::ZExt));
        BOOST_TEST(is_return_of(f.getEntryBlock().getTerminator(), unsigned_constant(4294967295ul)));
        return boost::test_tools::predicate_result(true);
    }));
    BOOST_TEST(has_function("f195", [] (llvm::Function const& f)
    {
        BOOST_TEST(f.hasAttribute(llvm::AttributeList::ReturnIndex, llvm::Attribute::ZExt));
        BOOST_TEST(is_return_of(f.getEntryBlock().getTerminator(), unsigned_constant(18446744073709551615ul)));
        return boost::test_tools::predicate_result(true);
    }));
    BOOST_TEST(has_function("f235", [] (llvm::Function const& f)
    {
        BOOST_TEST(f.hasAttribute(llvm::AttributeList::ReturnIndex, llvm::Attribute::ZExt));
        BOOST_TEST(is_return_of(f.getEntryBlock().getTerminator(), unsigned_constant(9999999999999999999ul)));
        return boost::test_tools::predicate_result(true);
    }));
    BOOST_TEST(has_function("f237", [] (llvm::Function const& f)
    {
        BOOST_TEST(f.hasAttribute(llvm::AttributeList::ReturnIndex, llvm::Attribute::ZExt));
        BOOST_TEST(is_return_of(f.getEntryBlock().getTerminator(), unsigned_constant(9999999999999999999ul)));
        return boost::test_tools::predicate_result(true);
    }));
    BOOST_TEST(has_function("f238", [] (llvm::Function const& f)
    {
        BOOST_TEST(f.hasAttribute(llvm::AttributeList::ReturnIndex, llvm::Attribute::ZExt));
        BOOST_TEST(is_return_of(f.getEntryBlock().getTerminator(), unsigned_constant(9999999999999999999ul)));
        return boost::test_tools::predicate_result(true);
    }));
}

// BOOST_AUTO_TEST_CASE(parse_error_000)

// BOOST_AUTO_TEST_CASE(typecheck_error_000)
// BOOST_AUTO_TEST_CASE(typecheck_error_001)
// BOOST_AUTO_TEST_CASE(typecheck_error_002)
// BOOST_AUTO_TEST_CASE(typecheck_error_003)
// BOOST_AUTO_TEST_CASE(typecheck_error_004)
// BOOST_AUTO_TEST_CASE(typecheck_error_005)
// BOOST_AUTO_TEST_CASE(typecheck_error_006)
// BOOST_AUTO_TEST_CASE(typecheck_error_007)
// BOOST_AUTO_TEST_CASE(typecheck_error_008)
// BOOST_AUTO_TEST_CASE(typecheck_error_009)
// BOOST_AUTO_TEST_CASE(typecheck_error_010)
// BOOST_AUTO_TEST_CASE(typecheck_error_011)
// BOOST_AUTO_TEST_CASE(typecheck_error_012)
// BOOST_AUTO_TEST_CASE(typecheck_error_013)
// BOOST_AUTO_TEST_CASE(typecheck_error_014)
// BOOST_AUTO_TEST_CASE(typecheck_error_015)
// BOOST_AUTO_TEST_CASE(typecheck_error_016)
// BOOST_AUTO_TEST_CASE(typecheck_error_017)
// BOOST_AUTO_TEST_CASE(typecheck_error_018)
// BOOST_AUTO_TEST_CASE(typecheck_error_019)
// BOOST_AUTO_TEST_CASE(typecheck_error_020)
// BOOST_AUTO_TEST_CASE(typecheck_error_021)
// BOOST_AUTO_TEST_CASE(typecheck_error_022)
// BOOST_AUTO_TEST_CASE(typecheck_error_023)
// BOOST_AUTO_TEST_CASE(typecheck_error_024)
// BOOST_AUTO_TEST_CASE(typecheck_error_025)
// BOOST_AUTO_TEST_CASE(typecheck_error_026)
// BOOST_AUTO_TEST_CASE(typecheck_error_027)
// BOOST_AUTO_TEST_CASE(typecheck_error_028)
// BOOST_AUTO_TEST_CASE(typecheck_error_029)
// BOOST_AUTO_TEST_CASE(typecheck_error_030)
// BOOST_AUTO_TEST_CASE(typecheck_error_031)
// BOOST_AUTO_TEST_CASE(typecheck_error_032)
// BOOST_AUTO_TEST_CASE(typecheck_error_033)
// BOOST_AUTO_TEST_CASE(typecheck_error_034)
// BOOST_AUTO_TEST_CASE(typecheck_error_035)
// BOOST_AUTO_TEST_CASE(typecheck_error_036)
// BOOST_AUTO_TEST_CASE(typecheck_error_037)
// BOOST_AUTO_TEST_CASE(typecheck_error_038)
// BOOST_AUTO_TEST_CASE(typecheck_error_039)
// BOOST_AUTO_TEST_CASE(typecheck_error_040)
// BOOST_AUTO_TEST_CASE(typecheck_error_041)
// BOOST_AUTO_TEST_CASE(typecheck_error_042)
// BOOST_AUTO_TEST_CASE(typecheck_error_043)
// BOOST_AUTO_TEST_CASE(typecheck_error_044)
// BOOST_AUTO_TEST_CASE(typecheck_error_045)
// BOOST_AUTO_TEST_CASE(typecheck_error_046)
// BOOST_AUTO_TEST_CASE(typecheck_error_047)
// BOOST_AUTO_TEST_CASE(typecheck_error_048)
// BOOST_AUTO_TEST_CASE(typecheck_error_049)
// BOOST_AUTO_TEST_CASE(typecheck_error_050)
// BOOST_AUTO_TEST_CASE(typecheck_error_051)
// BOOST_AUTO_TEST_CASE(typecheck_error_052)
// BOOST_AUTO_TEST_CASE(typecheck_error_053)
// BOOST_AUTO_TEST_CASE(typecheck_error_054)
// BOOST_AUTO_TEST_CASE(typecheck_error_055)
// BOOST_AUTO_TEST_CASE(typecheck_error_056)
// BOOST_AUTO_TEST_CASE(typecheck_error_057)
// BOOST_AUTO_TEST_CASE(typecheck_error_058)
// BOOST_AUTO_TEST_CASE(typecheck_error_059)
// BOOST_AUTO_TEST_CASE(typecheck_error_060)
// BOOST_AUTO_TEST_CASE(typecheck_error_061)
// BOOST_AUTO_TEST_CASE(typecheck_error_062)
// BOOST_AUTO_TEST_CASE(typecheck_error_063)
// BOOST_AUTO_TEST_CASE(typecheck_error_064)
// BOOST_AUTO_TEST_CASE(typecheck_error_065)
// BOOST_AUTO_TEST_CASE(typecheck_error_066)
// BOOST_AUTO_TEST_CASE(typecheck_error_067)
// BOOST_AUTO_TEST_CASE(typecheck_error_068)
// BOOST_AUTO_TEST_CASE(typecheck_error_069)
// BOOST_AUTO_TEST_CASE(typecheck_error_070)
// BOOST_AUTO_TEST_CASE(typecheck_error_071)
// BOOST_AUTO_TEST_CASE(typecheck_error_072)
// BOOST_AUTO_TEST_CASE(typecheck_error_073)
// BOOST_AUTO_TEST_CASE(typecheck_error_074)
// BOOST_AUTO_TEST_CASE(typecheck_error_075)
// BOOST_AUTO_TEST_CASE(typecheck_error_076)
// BOOST_AUTO_TEST_CASE(typecheck_error_077)
// BOOST_AUTO_TEST_CASE(typecheck_error_078)
// BOOST_AUTO_TEST_CASE(typecheck_error_079)
// BOOST_AUTO_TEST_CASE(typecheck_error_080)
// BOOST_AUTO_TEST_CASE(typecheck_error_081)
// BOOST_AUTO_TEST_CASE(typecheck_error_082)
// BOOST_AUTO_TEST_CASE(typecheck_error_083)
// BOOST_AUTO_TEST_CASE(typecheck_error_084)
// BOOST_AUTO_TEST_CASE(typecheck_error_085)
// BOOST_AUTO_TEST_CASE(typecheck_error_086)
// BOOST_AUTO_TEST_CASE(typecheck_error_087)
// BOOST_AUTO_TEST_CASE(typecheck_error_088)
// BOOST_AUTO_TEST_CASE(typecheck_error_089)
// BOOST_AUTO_TEST_CASE(typecheck_error_090)
// BOOST_AUTO_TEST_CASE(typecheck_error_091)
// BOOST_AUTO_TEST_CASE(typecheck_error_092)
// BOOST_AUTO_TEST_CASE(typecheck_error_093)
// BOOST_AUTO_TEST_CASE(typecheck_error_094)
// BOOST_AUTO_TEST_CASE(typecheck_error_095)
// BOOST_AUTO_TEST_CASE(typecheck_error_096)
// BOOST_AUTO_TEST_CASE(typecheck_error_097)
// BOOST_AUTO_TEST_CASE(typecheck_error_098)
// BOOST_AUTO_TEST_CASE(typecheck_error_099)
// BOOST_AUTO_TEST_CASE(typecheck_error_100)
// BOOST_AUTO_TEST_CASE(typecheck_error_101)
// BOOST_AUTO_TEST_CASE(typecheck_error_102)
// BOOST_AUTO_TEST_CASE(typecheck_error_103)
// BOOST_AUTO_TEST_CASE(typecheck_error_104)
// BOOST_AUTO_TEST_CASE(typecheck_error_105)
// BOOST_AUTO_TEST_CASE(typecheck_error_106)
// BOOST_AUTO_TEST_CASE(typecheck_error_107)
// BOOST_AUTO_TEST_CASE(typecheck_error_108)
// BOOST_AUTO_TEST_CASE(typecheck_error_109)
// BOOST_AUTO_TEST_CASE(typecheck_error_110)
// BOOST_AUTO_TEST_CASE(typecheck_error_111)
// BOOST_AUTO_TEST_CASE(typecheck_error_112)
// BOOST_AUTO_TEST_CASE(typecheck_error_113)
// BOOST_AUTO_TEST_CASE(typecheck_error_114)
// BOOST_AUTO_TEST_CASE(typecheck_error_115)
// BOOST_AUTO_TEST_CASE(typecheck_error_116)
// BOOST_AUTO_TEST_CASE(typecheck_error_117)
// BOOST_AUTO_TEST_CASE(typecheck_error_118)
// BOOST_AUTO_TEST_CASE(typecheck_error_119)
// BOOST_AUTO_TEST_CASE(typecheck_error_120)
// BOOST_AUTO_TEST_CASE(typecheck_error_121)
// BOOST_AUTO_TEST_CASE(typecheck_error_122)
// BOOST_AUTO_TEST_CASE(typecheck_error_123)
// BOOST_AUTO_TEST_CASE(typecheck_error_124)
// BOOST_AUTO_TEST_CASE(typecheck_error_125)

BOOST_AUTO_TEST_SUITE_END()
