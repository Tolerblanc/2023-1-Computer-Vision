# 2023-1 KWU Computer Vision Homework 02

- Submitter : HyunJun KIM (2019204054)
## Summary
- Draw Histogram, Histogram equalization, Image thresholding
- Created on Apple Silicon MacOSX 13.2, CMake Configuration may not working.
- If you are only using source files, please uncomment CMakeLists.txt to copy the image
## Installation
### Apple Silicon Mac
1. Install Homebrew  
2. Install pkg-config and OpenCV via Homebrew  
```bash
brew install pkg-config; brew install opencv
```
3. Complie main.cpp  
```bash
clang++ -std=c++11 'pkg-config opencv4 --libs --cflags opencv4' main.cpp
```
### Windows (may not working. not tested)
1. Install CMake and precomplied OpenCV in /project_root/third_party/
2. Configure CMake and build
