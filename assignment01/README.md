# 2023-1 KWU Computer Vision Homework 01

- Submitter : HyunJun KIM (2019204054)
## Summary
- Drawing rectangles, ellipses, brush with mouse and keyboard events
- Created on Apple Silicon MacOSX 13.2, CMake Configuration may not working.
## Installation
- Apple Silicon Mac
1. Install Homebrew  
2. Install pkg-config and OpenCV using Homebrew  
'''bash
brew install pkg-config; brew install opencv
'''
3. Complie main.cpp  
'''bash
clang++ -std=c++11 'pkg-config opencv4 --libs --cflags opencv4'
'''
- Windows (may not working. not tested)
1. Install CMake and precomplied OpenCV in /project_root/third_party/
2. Configure CMake and build
