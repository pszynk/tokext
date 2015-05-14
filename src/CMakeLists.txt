# libraries needed
SET (project_LIBS ${LOG4CXX_LIBRARIES})
# name of binaries
SET (project_BIN ${PROJECT_NAME})


# all headers and sources {{{
#FILE (GLOB_RECURSE project_SRCS RELATIVE . *.cpp *.cxx *.cc *.C *.c *.h *.hpp)
# file with main
SET (main_SRC "tokext/main_test.cpp")

# set other
SET (project_SRCS
    libraryinfo.h

    tokext/tokext.hpp

    tokext/ExtractionParams.hpp
    tokext/Flow.hpp
    tokext/GeneralizedSuffixArray.cpp
    tokext/GeneralizedSuffixArray.hpp
    tokext/InputBuffer.hpp
    tokext/InputData.cpp
    tokext/InputData.hpp
    tokext/IntegratedBuffer.cpp
    tokext/IntegratedBuffer.hpp
    tokext/Occurrence.cpp
    tokext/Occurrence.hpp
    tokext/Position.hpp
    tokext/SuffixContext.cpp
    tokext/SuffixContext.hpp
    tokext/Token.cpp
    tokext/Token.hpp
    tokext/TokenUtility.cpp
    tokext/TokenUtility.hpp
    tokext/TokextException.hpp
    tokext/lcs_algorithm.cpp
    tokext/lcs_algorithm.hpp

    tokext/skew.cpp
    tokext/skew.hpp
    tokext/types.hpp

    tokext/lv/Index.cpp
    tokext/lv/Index.hpp
    tokext/lv/Interval.cpp
    tokext/lv/Interval.hpp
    tokext/lv/LcpList.cpp
    tokext/lv/LcpList.hpp

    tokext/rmq/RmqWrapper.cpp
    tokext/rmq/RmqWrapper.hpp

    tokext/utils/extraction.cpp
    tokext/utils/extraction.hpp
    tokext/utils/translation.hpp

    tokext/testing/random_input.hpp
    )
#}}}

ADD_LIBRARY(corelib OBJECT ${project_SRCS})

if ( NOT DEFINED BUILD_SHARED_LIBS )
    SET (BUILD_SHARED_LIBS 0)
endif ( NOT DEFINED BUILD_SHARED_LIBS )

if( BUILD_SHARED_LIBS )
    ADD_LIBRARY(sharedlib SHARED ${project_SRCS})
    TARGET_LINK_LIBRARIES(sharedlib ${project_LIBS})
    SET_TARGET_PROPERTIES(
        sharedlib PROPERTIES
        VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}"
        OUTPUT_NAME ${project_BIN}
        CLEAN_DIRECT_OUTPUT 1)

    IF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )
        SET_TARGET_PROPERTIES(sharedlib PROPERTIES COMPILE_FLAGS "-fPIC")
    ENDIF( CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64" )

    INSTALL(TARGETS sharedlib DESTINATION lib)

else( BUILD_SHARED_LIBS )
    ADD_LIBRARY(staticlib STATIC $<TARGET_OBJECTS:corelib>)
    TARGET_LINK_LIBRARIES(staticlib ${project_LIBS})
    SET_TARGET_PROPERTIES(
        staticlib PROPERTIES
        VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}"
        OUTPUT_NAME ${project_BIN}
        CLEAN_DIRECT_OUTPUT 1)

    INSTALL(TARGETS staticlib DESTINATION lib)

endif( BUILD_SHARED_LIBS )

install(DIRECTORY ./ DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")

ADD_EXECUTABLE(exec ${main_SRC} $<TARGET_OBJECTS:corelib>)
TARGET_LINK_LIBRARIES(exec ${project_LIBS})
SET_TARGET_PROPERTIES(
    exec PROPERTIES
    VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}"
    OUTPUT_NAME ${project_BIN}
    CLEAN_DIRECT_OUTPUT 1)


