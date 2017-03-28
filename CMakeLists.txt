cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CustomCmake/")

 
SET( ProjectName 
    cobs
  )

project(${ProjectName})

find_package(BOOST)
include_directories (${Boost_INCLUDE_DIR}) 
link_directories(${Boost_LIBRARY_DIRS})
SET(SRCS 
	main.cpp
     )
	
SET(HDRS
	 	cobs.h)
	

add_executable(${cobsName} main.cpp ${SRCS})
