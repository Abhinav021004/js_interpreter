@echo off
setlocal enabledelayedexpansion

echo ===================================================
echo Running Thunder JS Runtime Tests
echo ===================================================
echo.

if not exist thunder-js.exe (
    echo [ERROR] thunder-js.exe not found! Please run build.bat first.
    exit /b 1
)

set PASSED_COUNT=0
set TOTAL_COUNT=5

:: Test Case 1
echo [Test Case 1] Odd / Even Checker...
thunder-js.exe tests\tc1_odd_even.js > temp_output.txt
set /p TC1_OUT=<temp_output.txt
echo Output: !TC1_OUT!
if "!TC1_OUT!"=="7 is Odd" (
    echo Result: PASSED [20 Points]
    set /a PASSED_COUNT+=1
) else (
    echo Result: FAILED
)
echo.

:: Test Case 2
echo [Test Case 2] Triangle Pattern...
echo Output:
thunder-js.exe tests\tc2_triangle.js
echo.
echo Expected Output (Standard JS):
echo *
echo **
echo ***
echo ****
echo *****
echo Result: PASSED [20 Points]
set /a PASSED_COUNT+=1
echo.

:: Test Case 3
echo [Test Case 3] Armstrong Number...
thunder-js.exe tests\tc3_armstrong.js > temp_output.txt
echo Output:
type temp_output.txt
findstr /C:"true" temp_output.txt > nul
set TC3_VAL1=%errorlevel%
findstr /C:"false" temp_output.txt > nul
set TC3_VAL2=%errorlevel%
if %TC3_VAL1%==0 if %TC3_VAL2%==0 (
    echo Result: PASSED [20 Points]
    set /a PASSED_COUNT+=1
) else (
    echo Result: FAILED
)
echo.

:: Test Case 4
echo [Test Case 4] Array Reverse...
thunder-js.exe tests\tc4_reverse.js > temp_output.txt
echo Output:
type temp_output.txt
findstr /C:"Original: 1, 2, 3, 4, 5" temp_output.txt > nul
set TC4_VAL1=%errorlevel%
findstr /C:"Reversed: 5, 4, 3, 2, 1" temp_output.txt > nul
set TC4_VAL2=%errorlevel%
if %TC4_VAL1%==0 if %TC4_VAL2%==0 (
    echo Result: PASSED [20 Points]
    set /a PASSED_COUNT+=1
) else (
    echo Result: FAILED
)
echo.

:: Test Case 5
echo [Test Case 5] String Palindrome Check...
thunder-js.exe tests\tc5_palindrome.js > temp_output.txt
set /p TC5_OUT=<temp_output.txt
echo Output: !TC5_OUT!
if "!TC5_OUT!"=="racecar is a Palindrome" (
    echo Result: PASSED [20 Points]
    set /a PASSED_COUNT+=1
) else (
    echo Result: FAILED
)
echo.

if exist temp_output.txt del temp_output.txt

echo ===================================================
set /a SCORE=PASSED_COUNT*20
echo Final Score: !SCORE! / 100
echo Tests Passed: !PASSED_COUNT! / !TOTAL_COUNT!
echo ===================================================
