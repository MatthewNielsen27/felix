#include <catch2/catch_test_macros.hpp>

#include "felix/String.hpp"

using namespace felix;

TEST_CASE("[felix::String] small strings are kept on the stack") {
    const auto str = String{"Hello!"};
    REQUIRE(str.is_small());
    REQUIRE(str.use_count() == 0);
}

TEST_CASE("[felix::String] copying a large string increases the refcount") {
    const auto str = String{"Lorem ipsum dolor sit amet, consectetur adipiscing elit"};
    REQUIRE_FALSE(str.is_small());

    CHECK(str.use_count() == 1);

    {
        const auto str2 = str;
        CHECK(str.use_count() == 2);
        CHECK(str.use_count() == str2.use_count());
    }

    // use_count() should decrease if other reference goes out of scope.
    CHECK(str.use_count() == 1);
}
