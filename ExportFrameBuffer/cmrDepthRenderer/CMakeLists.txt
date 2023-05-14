set(CMAKE_CXX_STANDARD 20)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(TARGET_NAME cmrDepthRenderer)
add_library(${TARGET_NAME} STATIC
    LibExport.h
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

set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/bin)	
set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/bin)
set_target_properties(${TARGET_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/lib)
set_target_properties(${TARGET_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/lib)