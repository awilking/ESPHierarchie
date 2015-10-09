# A.Bommer@allasoft.de
# cmake version required.
cmake_minimum_required(VERSION 2.8)

set( CMAKE_MODULE_PATH $ENV{HOME}/Modules)

# Project.
project (hierarchie CXX)

# Version.
set( HIERARCHIE_MAJOR_VERSION 1)
set( HIERARCHIE_MINOR_VERSION 0)
set( HIERARCHIE_PATCH_LEVEL 0)

add_subdirectory( src)

configure_file( log4crc.in log4crc @only)
configure_file( inc/version.h.in inc/version,h @only)

