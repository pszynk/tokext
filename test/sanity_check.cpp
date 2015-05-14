/*
 * This is an example unit test that doesn't really do anything useful.
 * It is here as a reference for you when creating additional unit tests.
 * For additional reference information, see the "test.h" header.
 */

#include "test.hpp" // Brings in the Catch framework

#include <random>

namespace
{


unsigned int Factorial( unsigned int number ) {
  return number > 1 ? Factorial(number-1)*number : 1;
}

TEST_CASE( "Factorials are computed", "[.][sanity]" ) {
    static std::mt19937 generator(getSeedValue());
    static int diff = 1;
    std::uniform_int_distribution<int> dist;
    SECTION("check random  - no 1", "")
    {
        int fromSec1 = dist(generator);
        REQUIRE(fromSec1 != diff);
        diff = fromSec1;
    }

    SECTION("check random - no 2", "")
    {
        REQUIRE(dist(generator) != diff);
    }

    SECTION("other simple", "")
    {
        REQUIRE( Factorial(0) == 1 );
        REQUIRE( Factorial(1) == 1 );
        REQUIRE( Factorial(2) == 2 );
        REQUIRE( Factorial(3) == 6 );
        REQUIRE( Factorial(10) == 3628800 );
    }
}

}
