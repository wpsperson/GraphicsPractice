cmake_minimum_required(VERSION 3.10.0)

project(OpenGL)
set(TARGET_NAME 2DRenderer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_MSVC_INCLUDE_FLAG_NO_EXTERNAL ON) # include directory use /I instead of /external:I
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
find_package(Qt5 CONFIG REQUIRED COMPONENTS Widgets)
find_package(GLEW REQUIRED)
find_package(FreeType REQUIRED)
#find_package(ZLIB REQUIRED)

add_executable(${TARGET_NAME}
    OpenGLHeader.h
    main.cpp 
    OpenGLWidget.h
    OpenGLWidget.cpp
    Box.h
    Box.cpp
    ViewPort.h
    ViewPort.cpp
    Renderer.h
    Renderer.cpp
    GLObject2D.h
    GLObject2D.cpp
    ProgramManager.cpp
    ProgramManager.h
    FontManager.cpp
    FontManager.h
    GlyphOutlineGenerator.h
    GlyphOutlineGenerator.cpp
    Const.h
    ShaderSource.h
    Geometry.h
    Geometry.cpp
    Operations/Operation.h
    Operations/Operation.cpp
    Operations/DemoOperation.h
    Operations/DemoOperation.cpp
    Operations/DefaultOperation.cpp
    Operations/EditVectorFontOperation.h
    Operations/EditVectorFontOperation.cpp
    Operations/UnicodeOutlineOperation.h
    Operations/UnicodeOutlineOperation.cpp
    Operations/FBOOperation.h
    Operations/FBOOperation.cpp
    Operations/PersistMapOperation.h
    Operations/PersistMapOperation.cpp
    Performance/CirclesColorAttrib.h
    Performance/CirclesColorAttrib.cpp
    Performance/CirclesColorUniform.h
    Performance/CirclesColorUniform.cpp
    AllChineseChar.h
    Utils.h
    Utils.cpp
    FreeTypeUtils.h
    FreeTypeUtils.cpp
)

get_target_property(ProjectSourceFiles ${TARGET_NAME} SOURCES)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ProjectSourceFiles})

target_include_directories(${TARGET_NAME} PRIVATE ${GLEW_INCLUDE_DIR})
target_include_directories(${TARGET_NAME} PRIVATE ${FREETYPE_INCLUDE_DIR})

if(WIN32)
    target_link_libraries(${TARGET_NAME} PRIVATE opengl32 glu32)
else ()
    target_link_libraries(${TARGET_NAME} PRIVATE GL GLU pthread)
endif ()

target_link_libraries(${TARGET_NAME} PRIVATE Qt5::Widgets ${CMAKE_DL_LIBS} ${GLEW_LIB})
target_link_libraries(${TARGET_NAME} PRIVATE debug "${FREETYPE_LIBRARY_DEBUG}" optimized "${FREETYPE_LIBRARY}")
#target_link_libraries(${TARGET_NAME} debug "${ZLIB_LIBRARY_DEBUG}" optimized "${ZLIB_LIBRARY}")
