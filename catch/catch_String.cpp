#include <catch2/catch_test_macros.hpp>

#include "felix/String.hpp"

using namespace felix;

TEST_CASE("[felix::String] small strings are kept on the stack") {
    const auto str = String{"Hello!"};
    REQUIRE(str.is_small());
    REQUIRE(str.use_count() == 0);
}

TEST_CASE("[felix::String] appending 2 small strings yields a small string (when appropriate)") {
    GIVEN("a small string") {
        const auto str = String{"a"};
        REQUIRE(str.is_small());

        WHEN("another small string is appended to it (without overflow)") {
            const auto result = str.append("b");

            THEN("the result should still be a small string") {
                CHECK(result.is_small());
                CHECK(result == String{"ab"});
            }
        }

        WHEN("a small string_view is appended to it (with overflow)") {
            const auto result = str.append("bcdefghi");

            THEN("the result should still be a small string") {
                CHECK(!result.is_small());
                CHECK(result == String{"abcdefghi"});
            }
        }

        WHEN("a small String is appended to it (with overflow)") {
            const auto result = str.append(String{"bcdefghi"});

            THEN("the result should still be a small string") {
                CHECK(!result.is_small());
                CHECK(result == String{"abcdefghi"});
            }
        }
    }
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

TEST_CASE("[felix::String] comparison operators") {
    CHECK(String{"hello world"} == String{"hello world"});
    CHECK(String{"hello world"} != String{"hello there"});
    CHECK(String{"hello world"} != String{""});
    CHECK(String{"hello world"} != String{"a"});
}
