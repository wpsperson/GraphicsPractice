set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(TARGET_NAME TestCase)
add_executable(${TARGET_NAME}
    main.cpp
)

target_include_directories(${TARGET_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/cmrDepthRenderer)
target_link_directories(${TARGET_NAME} PRIVATE ${PROJECT_BINARY_DIR}/lib)
target_link_libraries(${TARGET_NAME} PRIVATE ${GLFW_LIBRARY_DEBUG} ${ASSIMP_LIBRARY} cmrDepthRenderer.lib)
set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/bin)	
set_target_properties(${TARGET_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_BINARY_DIR}/bin)

