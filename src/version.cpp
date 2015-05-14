#include <tagext/version.h>
#include "libraryinfo.h"

EXTERNC int tagext_version_major()
{
    return APPLICATION_VERSION_MAJOR;
}

EXTERNC int tagext_version_minor()
{
    return APPLICATION_VERSION_MINOR;
}

EXTERNC int tagext_version_patch()
{
    return APPLICATION_VERSION_PATCH;
}
