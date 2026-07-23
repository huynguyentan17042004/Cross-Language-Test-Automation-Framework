#!/bin/bash
clear
BUILD_CMD="g++ -g -fsanitize=address -o main main.cpp dragon.cpp -I . -std=c++11"

echo "[BUILD] Compiling..."
echo "$BUILD_CMD"
eval $BUILD_CMD

if [ $? -eq 0 ]; then
    echo "[RUN] Running with AddressSanitizer..."

    ./main
    echo "[DONE]"

else
    echo "[BUILD FAILED]"
    exit 1
fi