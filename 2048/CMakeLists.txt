cmake_minimum_required(VERSION 3.5.1 )

project(2048)

add_compile_options(-std=c++11)

aux_source_directory(./src SOURCES)

include_directories( "src/headers/")

add_executable( 2048 ${SOURCES} )


