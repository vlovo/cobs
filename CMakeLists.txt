cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CustomCmake/")

 
SET( ProjectName 
    cobs
  )

project(${ProjectName})

find_package(BOOST)
include_directories (${Boost_INCLUDE_DIR}) 
link_directories(${Boost_LIBRARY_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

SET(SRCS 
	main.cpp
     )
	
SET(HDRS
	 	cobs.h)
	

add_executable(${ProjectName} main.cpp ${SRCS})
