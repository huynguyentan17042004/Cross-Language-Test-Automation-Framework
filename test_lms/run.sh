#!/bin/bash
reset

# echo "[PROBE] Running Python probe test..."
# python3 -u "test.py"

echo "[BUILD] Compiling C++ project with AddressSanitizer..."
BUILD_CMD="g++ -g -o main main.cpp dragon.cpp -I . -std=c++11"
# g++ -g -fsanitize=address -o main probe.cpp dragon.cpp -I . -std=c++11

echo "$BUILD_CMD"
eval $BUILD_CMD

if [ $? -eq 0 ]; then
    echo "[RUN] Running executable..."      
    ./main
    echo "[DONE]"
else
    echo "[BUILD FAILED]"
    exit 1
fi
