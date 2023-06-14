cmake_minimum_required(VERSION 3.11)

project(QtOpenGL)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 CONFIG REQUIRED COMPONENTS Widgets)

set(TARGET_NAME qt_opengl)
add_executable(${TARGET_NAME}
	main.cpp 
	CanvasWidget.h
	CanvasWidget.cpp
)

if(WIN32)
    target_link_libraries(${TARGET_NAME} PRIVATE opengl32 glu32)
else ()
    target_link_libraries(${TARGET_NAME} PRIVATE GL GLU pthread)
endif ()
target_link_libraries(${TARGET_NAME} PRIVATE Qt5::Widgets ${CMAKE_DL_LIBS})


