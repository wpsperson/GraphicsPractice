cmake_minimum_required(VERSION 3.10.0)
project(cmrDepthRenderer)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(GLFW MODULE REQUIRED)
find_package(ASSIMP MODULE REQUIRED)

ADD_SUBDIRECTORY(cmrDepthRenderer)
option(BuildTestCase "build test case" ON)
if (BuildTestCase)
	ADD_SUBDIRECTORY(TestCase)
endif(BuildTestCase)


