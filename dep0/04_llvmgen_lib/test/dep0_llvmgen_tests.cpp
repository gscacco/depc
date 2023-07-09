#define BOOST_TEST_MODULE dep0_llvmgen_tests
#include <boost/test/included/unit_test.hpp>

#include "dep0/parser/parse.hpp"
#include "dep0/llvmgen/gen.hpp"

#include <filesystem>
#include <cstdlib>

namespace dep0
{

std::ostream& operator<<(std::ostream& os, expected<parser::parse_tree> const& x)
{
    if (x) return os << "Parsing ok";
    else return pretty_print(os << std::endl, x.error());
}

}

std::string toString(llvm::Module const& m)
{
    std::string str;
    auto os = llvm::raw_string_ostream(str);
    m.print(os, nullptr);
    return str;
}

struct Fixture
{
    std::filesystem::path testfiles = std::getenv("DEP0_TESTFILES_DIR");
    llvm::LLVMContext llvm_ctx;

    dep0::typecheck::tree open(std::filesystem::path const& file)
    {
        auto const tree = dep0::parser::parse(testfiles / file);
        BOOST_TEST_REQUIRE(tree);
        auto const checked = dep0::typecheck::check(*tree);
        BOOST_TEST_REQUIRE(checked.has_value());
        return *checked;
    }
};

BOOST_FIXTURE_TEST_SUITE(dep0_llvmgen_tests, Fixture)

BOOST_AUTO_TEST_CASE(test_0000)
{
    auto const tree = open("test_0000.depc");
    auto const result = dep0::llvmgen::gen(llvm_ctx, "test.depc", tree.root);
    BOOST_TEST_REQUIRE(result.has_value());
    BOOST_TEST(result->get().size() == 0ul);
}

BOOST_AUTO_TEST_CASE(test_0001)
{
    auto const tree = open("test_0001.depc");
    auto const result = dep0::llvmgen::gen(llvm_ctx, "test.depc", tree.root);
    BOOST_TEST_REQUIRE(result.has_value());
    BOOST_TEST(result->get().size() == 1ul);
    auto const& f = *result->get().begin();
    auto const* p = result->get().getFunction("main");
    BOOST_TEST(&f == p);
}

BOOST_AUTO_TEST_SUITE_END()
