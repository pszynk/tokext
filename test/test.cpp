
#define CATCH_CONFIG_RUNNER
#include "test.hpp"

//#include <log4cxx/xml/domconfigurator.h>

#include <iostream>
#include <random>
//#include <regex>

/**
 * \brief Globally available seed value.
 **/
decltype( std::random_device().operator ()() ) g_seed_value = []()
{
    std::random_device rseed;
    return rseed();
}();

/**
 * \brief Returns a global seed.
 **/
decltype( std::random_device().operator ()() ) getSeedValue()
{
    return g_seed_value;
}

/**
 * \brief Parse the arguments and strip user defined seed parameters if they exist.
 * \param argc The number of arguments.
 * \param argv The program arguments.
 * \returns A vector of all Catch compatible arguments.
 **/
std::vector<char*> parseArguments( int argc, char* const argv[] )
{
    std::vector<char*> arguments;
    arguments.reserve( argc );
    arguments.push_back( argv[0] );

    //const std::regex rand_seed_regex( "(-sd|--seed)=([-+]?\\b\\d+\\b)" );
    std::string prefix("--seed=");

    for ( int i = 1; i < argc; ++i )
    {
        //std::cmatch rand_seed_match;
        //if ( std::regex_search( argv[i], rand_seed_match, rand_seed_regex ) )
        std::string arg(argv[i]);
        if (!arg.compare(0, prefix.size(), prefix))
        {
            auto seed_value = atoi(arg.substr(prefix.size()).c_str());
            std::stringstream ss;
            //ss << rand_seed_match[2];
            ss << seed_value;
            ss >> g_seed_value;
            continue;
        }
        arguments.push_back( argv[i] );
    }

    return arguments;
}

int main( int argc, char* const argv[] )
{
    //log4cxx::xml::DOMConfigurator::configure("test/Log4cxxConfig.xml");
    const auto catch_arguments = parseArguments( argc, argv );
    std::cout << "seed value: " << getSeedValue() << std::endl;

    return Catch::Session().run( static_cast<int>( catch_arguments.size() ), &catch_arguments.front() );
}
