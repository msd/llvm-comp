cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(BadCompiler VERSION 0.1.0 LANGUAGES C CXX)

find_package(LLVM REQUIRED)

llvm_map_components_to_libnames(LLVM_LIBS core)

add_executable(my_compiler mccomp.cpp)

target_include_directories(my_compiler PUBLIC ${LLVM_INCLUDE_DIRS})

target_link_directories(my_compiler PUBLIC ${LLVM_LIBRARY_DIRS})

target_compile_definitions(my_compiler PUBLIC ${LLVM_DEFINITIONS})

target_link_libraries(my_compiler PUBLIC ${LLVM_LIBS})
