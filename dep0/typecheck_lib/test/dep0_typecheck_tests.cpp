#define BOOST_TEST_MODULE dep0_typecheck_tests
#include <boost/test/included/unit_test.hpp>

#include "dep0/parser/parse.hpp"
#include "dep0/typecheck/check.hpp"

#include <filesystem>
#include <cstdlib>

using namespace dep0;

namespace dep0
{

std::ostream& operator<<(std::ostream& os, expected<parser::parse_tree> const& x)
{
    if (x) return os << "Parsing ok";
    else return pretty_print(os << std::endl, x.error());
}

}

struct Fixture
{
    std::filesystem::path testfiles = std::getenv("DEP0_TESTFILES_DIR");

    dep0::parser::parse_tree open(std::filesystem::path const& file)
    {
        auto const tree = dep0::parser::parse(testfiles / file);
        BOOST_TEST_REQUIRE(tree.has_value(), tree);
        return *tree;
    }
};

BOOST_FIXTURE_TEST_SUITE(dep0_typecheck_tests, Fixture)

BOOST_AUTO_TEST_CASE(test_0000)
{
    auto const tree = open("test_0000.depc");
    auto const result = dep0::typecheck::check(tree);
    BOOST_TEST(result.has_value());
}

BOOST_AUTO_TEST_CASE(test_0001)
{
    auto const tree = open("test_0001.depc");
    auto const result = dep0::typecheck::check(tree);
    BOOST_TEST(result.has_value());
}

BOOST_AUTO_TEST_CASE(test_0002)
{
    auto const tree = open("test_0002.depc");
    auto const result = dep0::typecheck::check(tree);
    BOOST_TEST_REQUIRE(result.has_error());
    BOOST_TEST(result.error().error == "Expecting expression of type 'int'");
}

BOOST_AUTO_TEST_SUITE_END()
