
cmake_minimum_required(VERSION 3.22.1)

project("decoder")

# 添加包下的所有文件到 library
aux_source_directory(interface SILK_INTERFACE)
aux_source_directory(src SILK_SRC)
aux_source_directory(include LAME_INCLUDE)
aux_source_directory(libmp3lame LAME_LIB)
aux_source_directory(util UTIL)
aux_source_directory(head HEAD)

add_library(${CMAKE_PROJECT_NAME} SHARED
        ${SILK_INTERFACE}
        ${SILK_SRC}
        ${LAME_INCLUDE}
        ${LAME_LIB}
        ${UTIL}
        ${HEAD}
        decoder.cpp)

include_directories( head util
        interface
        src
        include
        libmp3lame)

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        log)


