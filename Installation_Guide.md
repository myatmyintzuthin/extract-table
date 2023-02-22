# Installation Guide 
Operating System : Ubuntu 22.04 \
Build Tool : Bazel \
Compiler : Clang 

## Clang Installation
install Clang compiler version 11.
```
> sudo apt install clang-11
```
Setup Environment
```
> nano ~/.bashrc
```
add `alias clang=clang-11` at the end of ~/.bashrc.

```
> source .bashrc
```

## Bazel Installation
Install Bazel version 6.0.0

Download binary installer from this [link](https://github.com/bazelbuild/bazel/releases/download/6.0.0/bazel-6.0.0-installer-linux-x86_64.sh)

```
> mkdir local/bazel
> cd local/bazel
local/bazel > chmod +x bazel-6.0.0-installer-linux-x86_64.sh
local/bazel > ./bazel-6.0.0-installer-linux-x86_64.sh
```
Setup Environment
```
local/bazel > cd
> nano ~/.bashrc  
```  
add `export PATH="$PATH:$HOME/bin"` 
```
> source .bashrc
```   
Check bazel installation
```
> bazel --version
```
Installation is successful if bazel version is displayed.

## OpenCV Installation

OpenCV version : 4.7.0

Download opencv-4.7.0 from this [link](https://opencv.org/releases/).

Download opencv_contrib-4.7.0 from this [link](https://github.com/opencv/opencv_contrib/releases/tag/4.7.0)

```
> mkdir local/opencv
> cd local/opencv
local/opencv > unzip opencv_contrib-4.7.0.zip
local/opencv > unzip opencv-4.7.0.zip
```
Set up environment [optional]
```
> sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev 
> sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev 
```
Build OpenCV
```
> cd local/opencv/opencv-4.7.0
opencv-4.7.0 > mkdir build
opencv-4.7.0 > cd build
```
```
build > cmake \
    -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules \
    -D CMAKE_BUILD_TYPE=Release \
    -D CMAKE_INSTALL_PREFIX=/usr/local \
    .. 

```
```
build > make -j8 2>&1 | tee make.log
```

Install OpenCV
```
build > sudo make install
build > sudo ldconfig
```

Check OpenCV Installation
```
build > cd/local/lib
lib > ls
```
Installation is successful if libopencv_xxx.so is found.

## Json Package Installation 

```
> sudo apt install nlohmann-json3-dev
```
## VS Code `include_path` Setup

check `.vscode/c_cpp_properties.json`.

