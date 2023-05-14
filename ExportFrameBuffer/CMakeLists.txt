cmake_minimum_required(VERSION 3.10.0)
project(ExportFrameBuffer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(GLFW MODULE REQUIRED)
find_package(ASSIMP MODULE REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(TARGET_NAME cmrDepthRenderer)
add_executable(${TARGET_NAME}
    main.cpp
    cmrDepthRenderer.h
    cmrDepthRenderer.cpp
    ModelData.cpp
    ModelData.h
    GLObject.cpp
    GLObject.h
    Box.cpp
    Box.h
    AssimpAdaptor.cpp
    AssimpAdaptor.h
    ProgramManager.cpp
    ProgramManager.h
    ShaderDefine.h
    Const.h
    glad.c
)

target_include_directories(${TARGET_NAME} PRIVATE ${GLFW_INCLUDE_DIR})
target_include_directories(${TARGET_NAME} PRIVATE ${ASSIMP_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE ${GLFW_LIBRARY_DEBUG} ${ASSIMP_LIBRARY} ${CMAKE_DL_LIBS})


