#!/bin/bash

# Stop immediately if any command fails
set -e

# Compiler and settings
CXX=clang++
CXXFLAGS="-std=c++17 -stdlib=libc++"
OPENCV_FLAGS=$(pkg-config --cflags --libs opencv4)
SDK_PATH=$(xcrun --show-sdk-path)

# Explicit include path for standard library headers (Step 5 fix)
STD_INCLUDE="$SDK_PATH/usr/include/c++/v1"

# Output file
OUT=simulation

echo "🔧 Compiling $OUT using Step 5 fix..."
$CXX $CXXFLAGS simulation.cpp -o $OUT $OPENCV_FLAGS \
    -I$STD_INCLUDE -isysroot $SDK_PATH

echo "Build complete!"

