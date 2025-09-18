@echo off
echo Political Quiz Compiler
echo ======================
echo.

REM Change to the directory where the batch file is located
cd /d "%~dp0"

echo Current directory: %CD%
echo.

echo Checking file structure...
if not exist src\main.cpp (
    echo ERROR: main.cpp not found in src directory
    echo Looking for: %CD%\src\main.cpp
    dir src 2>nul || echo src directory does not exist
    pause
    exit /b
)

if not exist src\quiz.cpp (
    echo ERROR: quiz.cpp not found in src directory
    echo Looking for: %CD%\src\quiz.cpp
    dir src 2>nul || echo src directory does not exist
    pause
    exit /b
)

if not exist include\quiz.h (
    echo ERROR: quiz.h not found in include directory
    echo Looking for: %CD%\include\quiz.h
    dir include 2>nul || echo include directory does not exist
    pause
    exit /b
)

echo.
echo All files found:
echo - src\main.cpp
echo - src\quiz.cpp
echo - include\quiz.h
echo.

echo Checking compiler...
g++ --version
if %errorlevel% neq 0 (
    echo.
    echo ERROR: g++ compiler not found!
    echo Please install MinGW-w64 or check your PATH.
    pause
    exit /b
)

echo.
echo Compiling political quiz...
g++ -std=c++11 -I./include src/main.cpp src/quiz.cpp -o political_quiz.exe

if %errorlevel% equ 0 (
    echo.
    echo Compilation successful!
    echo.
    echo Running the program...
    echo.
    political_quiz.exe
) else (
    echo.
    echo Compilation failed!
    echo.
    echo Showing compilation errors:
    g++ -std=c++11 -I./include src/main.cpp src/quiz.cpp -o political_quiz.exe 2>&1
    echo.
    pause
)