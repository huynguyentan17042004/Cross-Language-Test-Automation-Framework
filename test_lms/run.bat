@echo off
g++ -g -o main main.cpp dragons.cpp -I . -std=c++11
if %errorlevel% neq 0 exit /b %errorlevel%
main.exe
