cmake_minimum_required(VERSION 2.8)
project(yircimg)
if(UNIX)
    add_definitions(-std=c++0x)
endif(UNIX)

find_package(JPEG)
if(JPEG_FOUND)
   include_directories(${JPEG_INCLUDE_DIR})
   add_executable(yircimg_den1 Main.cpp Image.hpp ImageChannel.hpp
               BasicJpegAdapter.hpp BasicJpegAdapter.cpp
               Algorithms/Block.hpp Algorithms/Bars.hpp Algorithms/Shrink.hpp Algorithms/Convert.hpp
               Algorithms/Contrast.hpp Algorithms/AutoContrast.hpp Algorithms/Invert.hpp
               TraitsClasses Color.hpp Algorithms/ModifiedAutoContrast.hpp Algorithms/Convolve.hpp)
   target_link_libraries(yircimg_den1 ${JPEG_LIBRARIES})
endif(JPEG_FOUND)
