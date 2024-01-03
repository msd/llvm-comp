cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(BadCompiler VERSION 0.1.0 LANGUAGES C CXX)

find_package(LLVM REQUIRED)

add_library(LLVMlib INTERFACE)
target_include_directories(LLVMlib INTERFACE ${LLVM_INCLUDE_DIRS})
target_link_directories(LLVMlib INTERFACE ${LLVM_LIBRARY_DIRS})
separate_arguments(LLVM_DEFINITIONS)
target_compile_definitions(LLVMlib INTERFACE ${LLVM_DEFINITIONS}) 
llvm_map_components_to_libnames(LLVM_LIBS core)
target_link_libraries(LLVMlib INTERFACE ${LLVM_LIBS})

file(GLOB SRC_FILES ${CMAKE_SOURCE_DIR}/*.cpp)
add_executable(my_compiler ${SRC_FILES})

target_compile_features(my_compiler PUBLIC cxx_std_17)
target_link_libraries(my_compiler PUBLIC LLVMlib)
