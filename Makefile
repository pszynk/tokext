##############################################################
#               CMake Project Wrapper Makefile               #
############################################################## 

SHELL := /bin/bash
RM    := rm -rf
MKDIR := mkdir -p

CMAKE_FLAGS = -DCMAKE_INSTALL_PREFIX=~/.local -DBUILD_SHARED_LIBS=1

BUILD_DIR = #build
MKF = Makefile

debug: BUILD_DIR = Debug/
debug: CMAKE_FLAGS += -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
debug: BUILD_MKF = $(addprefix $(BUILD_DIR), $(MKF))
debug: build_project

release: BUILD_DIR = Release/
release: CMAKE_FLAGS += -DCMAKE_BUILD_TYPE=Release
release: BUILD_MKF = $(addprefix $(BUILD_DIR), $(MKF))
release: build_project

build_project: $(BUILD_MKF)
	@ $(MKDIR) $(BUILD_DIR)
	@ (cd $(BUILD_DIR) >/dev/null 2>&1 && cmake $(CMAKE_FLAGS) ..)
	#$(MAKE) -C $(BUILD_DIR)

distclean:
	@- (cd build >/dev/null 2>&1 && cmake .. >/dev/null 2>&1)
	@- $(MAKE) --silent -C build clean || true
	@- $(RM) ./build/Makefile
	@- $(RM) ./build/src
	@- $(RM) ./build/test
	@- $(RM) ./build/CMake*
	@- $(RM) ./build/cmake.*
	@- $(RM) ./build/*.cmake
	@- $(RM) ./build/*.txt
	@- $(RM) ./docs/*.html
	@- $(RM) ./docs/*.css
	@- $(RM) ./docs/*.png
	@- $(RM) ./docs/*.jpg
	@- $(RM) ./docs/*.gif
	@- $(RM) ./docs/*.tiff
	@- $(RM) ./docs/*.php
	@- $(RM) ./docs/search
	@- $(RM) ./docs/installdox


#ifeq ($(findstring distclean,$(MAKECMDGOALS)),)

    #$(MAKECMDGOALS): ./build/Makefile
	#@ $(MAKE) -C build $(MAKECMDGOALS)

#endif
