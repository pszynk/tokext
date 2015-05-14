################################################################################
#
# CMake script for finding Tokext.
# The default CMake search process is used to locate files.
#
# This script creates the following variables:
#  TOKEXT_FOUND: Boolean that indicates if the package was found
#  TOKEXT_INCLUDE_DIRS: Paths to the necessary header files
#  TOKEXT_LIBRARIES: Package libraries
#  TOKEXT_LIBRARY_DIRS: Path to package libraries
#
################################################################################

include(FindPackageHandleStandardArgs)

# See if TOKEXT_ROOT is not already set in CMake
IF (NOT TOKEXT_ROOT)
    # See if TOKEXT_ROOT is set in process environment
    IF ( NOT $ENV{TOKEXT_ROOT} STREQUAL "" )
        SET (TOKEXT_ROOT "$ENV{TOKEXT_ROOT}")
	MESSAGE(STATUS "Detected TOKEXT_ROOT set to '${TOKEXT_ROOT}'")
    ENDIF ()
ENDIF ()

# If TOKEXT_ROOT is available, set up our hints
IF (TOKEXT_ROOT)
    SET (TOKEXT_INCLUDE_HINTS HINTS "${TOKEXT_ROOT}/include" "${TOKEXT_ROOT}")
    SET (TOKEXT_LIBRARY_HINTS HINTS "${TOKEXT_ROOT}/lib")
ENDIF ()

# Find headers and libraries
find_path(TOKEXT_INCLUDE_DIR NAMES tokext/tokext.hpp ${TOKEXT_INCLUDE_HINTS})
find_library(TOKEXT_LIBRARY NAMES tokext ${TOKEXT_LIBRARY_HINTS})
#find_library(TOKEXTD_LIBRARY NAMES tokext${CMAKE_DEBUG_POSTFIX} ${TOKEXT_LIBRARY_HINTS})

# Set TOKEXT_FOUND honoring the QUIET and REQUIRED arguments
find_package_handle_standard_args(TOKEXT DEFAULT_MSG TOKEXT_LIBRARY TOKEXT_INCLUDE_DIR)

# Output variables
if(TOKEXT_FOUND)
  # Include dirs
  set(TOKEXT_INCLUDE_DIRS ${TOKEXT_INCLUDE_DIR})

  # Libraries
  if(TOKEXT_LIBRARY)
    set(TOKEXT_LIBRARIES optimized ${TOKEXT_LIBRARY})
  else(TOKEXT_LIBRARY)
    set(TOKEXT_LIBRARIES "")
  endif(TOKEXT_LIBRARY)
  #if(TOKEXTD_LIBRARY)
  #  set(TOKEXT_LIBRARIES debug ${TOKEXTD_LIBRARY} ${TOKEXT_LIBRARIES})
  #endif(TOKEXTD_LIBRARY)

  # Link dirs
  get_filename_component(TOKEXT_LIBRARY_DIRS "${TOKEXT_LIBRARY}" DIRECTORY)
endif()

# Advanced options for not cluttering the cmake UIs
mark_as_advanced(TOKEXT_INCLUDE_DIR TOKEXT_LIBRARY)
