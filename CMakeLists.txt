cmake_minimum_required(VERSION 3.20)

project (simple_moving_average)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -lrt")

add_executable(simple_moving_average
	main.cpp	
)
