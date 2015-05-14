/*
 * This is an example unit test that doesn't really do anything useful.
 * It is here as a reference for you when creating additional unit tests.
 * For additional reference information, see the "test.h" header.
 */

#include "test.hpp" // Brings in the Catch framework

#include <random>
#include <sstream>

namespace
{

unsigned int Factorial( unsigned int number ) {
  return number > 1 ? Factorial(number-1)*number : 1;
}

TEST_CASE( "Nested SECTION test", "[.][nested]" ) {
    using namespace std;
    static mt19937 generator(getSeedValue());

    for (int i = 0; i < 10; ++i) {

        stringstream oss;
        oss << "outer " << i;
        SECTION(oss.str(), "")
        {
            for (int j = 0; j < 10; ++j) {
                stringstream oss;
                oss << "inner " << /*i <<*/ "|" << j;
                SECTION(oss.str(), "")
                {
                    REQUIRE( Factorial(0) == 1 );
                }
            }
        }

    }
}

}
