cmake_minimum_required(VERSION 3.0)
project(basic_jpeg_lib)
if(UNIX)
    add_definitions(-std=c++17)
endif(UNIX)

find_package(JPEG REQUIRED)
if(JPEG_FOUND)
   include_directories(${JPEG_INCLUDE_DIR})
   add_executable(
	test_project 
	Main.cpp 
	Image.hpp 
	ImageChannel.hpp
    BasicJpegAdapter.hpp 
	BasicJpegAdapter.cpp
    Algorithms/Block.hpp 
	Algorithms/Bars.hpp 
	Algorithms/Shrink.hpp 
	Algorithms/Convert.hpp
    Algorithms/Contrast.hpp 
	Algorithms/AutoContrast.hpp 
	Algorithms/Invert.hpp
    TraitsClasses.hpp 
	Color.hpp 
	Algorithms/ModifiedAutoContrast.hpp 
	Algorithms/Convolve.hpp)
   target_link_libraries(test_project ${JPEG_LIBRARIES})
endif(JPEG_FOUND)
