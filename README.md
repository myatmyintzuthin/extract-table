# Table Cell Extraction
![OpenCV](https://img.shields.io/badge/opencv-%23white.svg?style=for-the-badge&logo=opencv&logoColor=white)
![NumPy](https://img.shields.io/badge/numpy-%23013243.svg?style=for-the-badge&logo=numpy&logoColor=white)
![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)

This repository consists of a simple image processing C++ implementation to extract coordinate of each cell in a table image. \
The script can extract tables irrespective of image size. 

The result will be contour image and json output with position of each cell.

## Installation
for environment setup and installation go to [installation_guide](Installation_Guide.md).

## Usage
Complie and build:

```code
bazel build //TableExtract:extract-table
```
Run:
```code
bazel-bin/TableExtract/extract-table [image_path]
```



