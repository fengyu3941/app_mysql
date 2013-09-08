#---Check if cmake has the required version-----------------------------------------------------
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

#---Set name of the project to "ROOT". Has to be done after check of cmake version--------------
PROJECT(app_mysql)

#---Where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked-------------
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

#---Recurse into the given subdirectories.  This does not actually cause another cmake executable
#  to run. The same process will walk through the project's entire directory structure.
ADD_SUBDIRECTORY(src)

