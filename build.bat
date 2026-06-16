@echo off
echo ===================================================
echo Building Thunder JS Runtime (C++ + QuickJS-NG)
echo ===================================================

echo Compiling QuickJS Core Engine (C)...
gcc -O2 -c quickjs/quickjs-amalgam.c -o quickjs/quickjs.o -D_GNU_SOURCE -D_WIN32_WINNT=0x0601

if %errorlevel% neq 0 (
    echo [ERROR] Failed to compile QuickJS Engine.
    exit /b %errorlevel%
)

echo Compiling C++ Wrapper and Linking...
g++ -O2 src/main.cpp quickjs/quickjs.o -o thunder-js.exe -lm -lws2_32 -D_WIN32_WINNT=0x0601

if %errorlevel% neq 0 (
    echo [ERROR] Failed to compile C++ wrapper.
    exit /b %errorlevel%
)

echo [SUCCESS] Built thunder-js.exe successfully!
echo ===================================================
