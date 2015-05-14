
#include <catch/catch.hpp>

decltype( std::random_device().operator ()() ) getSeedValue();
