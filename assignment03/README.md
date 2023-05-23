# 2023-1 KWU Computer Vision Homework 03

- Submitter : HyunJun KIM (2019204054)
## Summary
- 
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
