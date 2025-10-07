cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

project(Mcppc CXX)

add_executable(
    ${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/Main.cpp
    ${PROJECT_SOURCE_DIR}/Calculator_Loop.cpp
    ${PROJECT_SOURCE_DIR}/Calculator_Loop.h
    ${PROJECT_SOURCE_DIR}/Print.h
    ${PROJECT_SOURCE_DIR}/Match.h
    ${PROJECT_SOURCE_DIR}/Util.h
    ${PROJECT_SOURCE_DIR}/Double_Operations.cpp
    ${PROJECT_SOURCE_DIR}/Double_Operations.h
)

if (MINGW AND CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    target_link_libraries(${PROJECT_NAME} stdc++exp)
endif()

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 23)