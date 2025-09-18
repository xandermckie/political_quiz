@echo off
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
echo Checking file structure...
if not exist src\main.cpp (
    echo ERROR: main.cpp not found in src directory
    pause
    exit /b
)
if not exist src\quiz.cpp (
    echo ERROR: quiz.cpp not found in src directory
    pause
    exit /b
)
if not exist include\quiz.h (
    echo ERROR: quiz.h not found in include directory
    pause
    exit /b
)

echo.
echo All files found. Compiling political quiz...
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