cmake_minimum_required(VERSION 3.0.0)
project(Translator VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(HEADER_FILES ${INCLUDE_DIR}/Lexer.h ${INCLUDE_DIR}/AST.h ${INCLUDE_DIR}/Translator.h)
set(SOURCES src/main.cpp src/Lexer.cpp src/AST.cpp ${HEADER_FILES})
add_executable(Translator ${SOURCES})

target_include_directories(Translator PRIVATE ${INCLUDE_DIR})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
