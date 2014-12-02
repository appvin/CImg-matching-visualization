CImg-Matching-Visualization
================

This project is a test project visualizing a set of corresponding points on 2 input images using CImg library (http://cimg.sourceforge.net/).

Required libraries:
- CMake 2.8.12.2
- CImg 1.5.7
Note that the project may not work with different versions of the libraries.

The project contains
- a single c++ code, main.cpp
- cmake related files, CMakeLists.txt and extern/FindCImg.cmake
- 2 input images, build/img1.ppm and build/img2.ppm

The main.cpp uses CImg library to
- load the input images, build/img1.ppm and build/img2.ppm
- synthesize a set of random corresponding points and their energy
- display the corresponding points
- save the matching result as out.png

To run the code,
- $ cd build
- $ cmake ..
- $ make
- $ ./CImgMatchingVisualization