#include <catch/catch.hpp>

#include "pystr.h"

TEST_CASE("Copy constructor doesn't create a new string", "[string(string&)]")
{
    py::string str1("Hello!");
    py::string str2(str1);

    REQUIRE(str1.begin() == str2.begin());
}

TEST_CASE("Moved class reset to an empty string", "string(string&&)")
{
    py::string str1("Hello!");
    py::string str2(std::move(str1));

    REQUIRE(str2 == "Hello!");
    REQUIRE(str1 == "");
    REQUIRE(str1.begin() == py::g_empty.begin());
}

TEST_CASE("'substr' doesn't copy unnecessarily", "[substr]")
{
    py::string str("Hello!");
    py::string s1 = str.substr(0, 6);
    py::string s2 = str.substr(0, 7);

    REQUIRE(s1.begin() == str.begin());
    REQUIRE(s2.begin() == str.begin());
}

TEST_CASE("'endswith' works", "[endswith]")
{
    py::string str("1234567890");

    REQUIRE(str.endswith("890"));
    REQUIRE(str.endswith("789", 0, 9));
    REQUIRE(str.endswith("789", 6, 9));
    REQUIRE(str.endswith("890", 6, 30));
    REQUIRE(!str.endswith("89"));
    REQUIRE(!str.endswith("X1234567890"));
    REQUIRE(!str.endswith("1234567890X"));
    REQUIRE(!str.endswith("X1234567890X"));
}

TEST_CASE("Test 'end' parameter with startswith", "[endswith]")
{
    py::string str("abcd");

    REQUIRE(str.endswith("cd", 0, 5));
    REQUIRE(str.endswith("cd", 0, 4));
    REQUIRE(!str.endswith("cd", 0, 3));
}

TEST_CASE("'startswith' works", "[endswith]")
{
    py::string s("1234567890");

    REQUIRE(s.startswith("1234"));
    REQUIRE(s.startswith("1234", 0, 5));
    REQUIRE(s.startswith("2345", 1, 5));
    REQUIRE(!s.startswith("1234", 1));
    REQUIRE(!s.startswith("X1234", 0));
}

TEST_CASE("'strip' doesn't copy unnecessarily", "[strip]")
{
    py::string str1("Hello!");
    py::string str2 = str1.strip();
    REQUIRE(str1.begin() == str2.begin());
}

TEST_CASE("'strip' works when the left strip consume all the string", "[strip]")
{
    py::string str1("aaa");
    py::string str2 = str1.strip("a");
    REQUIRE(str2 == "");
}

TEST_CASE("'strip' works when the parameters is not ordered", "[strip]")
{
    py::string str1("abcd");
    py::string str2 = str1.strip("cdb");
    REQUIRE(str2 == "a");
}

TEST_CASE("'strip' works", "[strip]")
{
    py::string s("   hey   ");
    REQUIRE(s.strip() == "hey");
}

TEST_CASE("Strip without effect doesn't create a new string", "[strip]")
{
    py::string str1("Hello!");
    py::string str2 = str1.strip();
    REQUIRE(str1 == str2);
    REQUIRE(str1.begin() == str2.begin());
}

TEST_CASE("'lstrip' works", "[lstrip]")
{
    py::string s("   hey");
    REQUIRE(s.lstrip() == "hey");
}

TEST_CASE("'rstrip' works", "[rstrip]")
{
    py::string s("hey   ");
    REQUIRE(s.rstrip() == "hey");
}

TEST_CASE("'sprintf' works", "[sprintf]")
{
    py::string str = py::string::sprintf("%s player: %d", "Hello", 10);
    REQUIRE(str == "Hello player: 10");
}
