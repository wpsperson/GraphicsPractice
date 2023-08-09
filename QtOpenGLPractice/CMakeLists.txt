cmake_minimum_required(VERSION 3.10.0)

project(OpenGL)
set(TARGET_NAME 2DRenderer)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)
find_package(Qt5 CONFIG REQUIRED COMPONENTS Widgets)
find_package(FreeType REQUIRED)


add_executable(${TARGET_NAME}
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
	Const.h
    ShaderSource.h
	Geometry.h
	Geometry.cpp
	Operation.h
	Operation.cpp
	DemoOperation.h
	DemoOperation.cpp
    Utils.h
    Utils.cpp
	glad.c
)

target_include_directories(${TARGET_NAME} PRIVATE ${FREETYPE_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} Qt5::Widgets ${CMAKE_DL_LIBS})
target_link_libraries(${TARGET_NAME} debug "${FREETYPE_LIBRARY_DEBUG}" optimized "${FREETYPE_LIBRARY}")
