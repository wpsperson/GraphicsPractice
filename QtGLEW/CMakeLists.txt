cmake_minimum_required(VERSION 3.10.0)

project(QtGLEW)
set(TARGET_NAME QtGLEW)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH  ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

find_package(Qt5 CONFIG REQUIRED COMPONENTS Widgets)
find_package(GLEW REQUIRED)

add_executable(${TARGET_NAME}
	main.cpp
    MainWindow.h
    MainWindow.cpp
    DummyWidget.h
    DummyWidget.cpp
	OpenGLWidget.h
	OpenGLWidget.cpp
	Renderer.h
	Renderer.cpp
)

target_include_directories(${TARGET_NAME} PRIVATE ${GLEW_INCLUDE_DIR})

if(WIN32)
    target_link_libraries(${TARGET_NAME} PRIVATE opengl32 glu32)
else ()
    target_link_libraries(${TARGET_NAME} PRIVATE GL GLU pthread)
endif ()
target_link_libraries(${TARGET_NAME} PRIVATE Qt5::Widgets ${CMAKE_DL_LIBS} ${GLEW_LIB})