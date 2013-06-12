# - Find libconfig
# Find the native libconfig includes and library
#
#  LIBBOOSTREGEX_INCLUDE_DIRS - where to find libconfig.h
#  LIBBOOSTREGEX_LIBRARIES    - List of libraries when using libconfig.
#  LIBBOOSTREGEX_FOUND        - True if libconfig found.


find_path( LIBBOOSTREGEX_INCLUDE_DIR boost/regex.hpp
	PATHS "/usr/include"
		  "/usr/local/include"
		  "~/usr/include"
          "~/usr/local/include"
          "/Rational/include")
		
find_library( LIBBOOSTREGEX_LIBRARY NAMES libboost_regex.a
	PATHS "/usr/lib"
		  "/usr/local/lib"
		  "~/usr/lib"
          "~/usr/local/lib"
          "/Rational/lib")

mark_as_advanced( LIBBOOSTREGEX_LIBRARY LIBBOOSTREGEX_INCLUDE_DIR )

if( LIBBOOSTREGEX_INCLUDE_DIR AND EXISTS "${LIBBOOSTREGEX_INCLUDE_DIR}/boost/regex.hpp" )
	file( STRINGS "${LIBBOOSTREGEX_INCLUDE_DIR}/mpitopoinfo.h" LIBBOOSTREGEX_H REGEX "^#define[ \t]+LIBBOOSTREGEX_VER_M[A-Z]+[ \t]+[0-9]+.*$" )
	string( REGEX REPLACE "^.*LIBBOOSTREGEX_VER_MAJOR[ \t]+([0-9]+).*$" "\\1" LIBBOOSTREGEX_MAJOR "${BOOSTREGEX_H}" )
	string( REGEX REPLACE "^.*LIBBOOSTREGEX_VER_MINOR[ \t]+([0-9]+).*$" "\\1" LIBBOOSTREGEX_MINOR  "${BOOSTREGEX_H}" )

	set( LIBBOOSTREGEX_VERSION_STRING "${LIBBOOSTREGEX_MAJOR}.${LIBBOOSTREGEX_MINOR}" )
	set( LIBBOOSTREGEX_VERSION_MAJOR "${LIBBOOSTREGEX_MAJOR}" )
	set( LIBBOOSTREGEX_VERSION_MINOR "${LIBBOOSTREGEX_MINOR}" )
endif()

# handle the QUIETLY and REQUIRED arguments and set LIBBOOSTREGEX_FOUND to TRUE if 
# all listed variables are TRUE
include( FindPackageHandleStandardArgs )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( LIBBOOSTREGEX
	REQUIRED_VARS LIBBOOSTREGEX_LIBRARY LIBBOOSTREGEX_INCLUDE_DIR
	VERSION_VAR LIBBOOSTREGEX_VERSION_STRING )

if( LIBBOOSTREGEX_FOUND )
	set( LIBBOOSTREGEX_LIBRARIES ${LIBBOOSTREGEX_LIBRARY} )
	set( LIBBOOSTREGEX_INCLUDE_DIRS ${LIBBOOSTREGEX_INCLUDE_DIR} )
endif()
