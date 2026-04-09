@echo off
setlocal

REM ==============================================
REM build.bat - CMake Build Script for WinLineZ
REM ==============================================
REM Usage:
REM   build.bat           - Build Release
REM   build.bat debug     - Build Debug
REM   build.bat clean     - Clean build artifacts
REM   build.bat run       - Build and run
REM   build.bat help      - Show help
REM ==============================================

set BUILD_DIR=build
set BIN_DIR=bin
set TARGET=mylinez
set "CMAKE=C:\Program Files\CMake\bin\cmake.exe"

REM Add CMake to PATH
set "PATH=%CMAKE%;%PATH%"

if "%1"=="clean" goto clean
if "%1"=="debug" goto debug
if "%1"=="run" goto run
if "%1"=="help" goto help
if "%1"=="" goto release

:release
echo.
echo ==============================================
echo Building %TARGET% (Release)
echo ==============================================
echo.
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"
"%CMAKE%" .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
if errorlevel 1 (
    cd ..
    echo CMake configuration failed!
    exit /b 1
)
cd ..
cd "%BUILD_DIR%"
"%CMAKE%" --build . --parallel 8
if errorlevel 1 (
    cd ..
    echo Build failed!
    exit /b 1
)
cd ..
echo.
echo Build Successful! Executable: %BIN_DIR%\%TARGET%.exe
echo.
goto end

:debug
echo.
echo ==============================================
echo Building %TARGET% (Debug)
echo ==============================================
echo.
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd "%BUILD_DIR%"
"%CMAKE%" .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug
if errorlevel 1 (
    cd ..
    echo CMake configuration failed!
    exit /b 1
)
cd ..
cd "%BUILD_DIR%"
"%CMAKE%" --build . --parallel 8
if errorlevel 1 (
    cd ..
    echo Build failed!
    exit /b 1
)
cd ..
echo.
echo Debug Build Successful! Executable: %BIN_DIR%\%TARGET%.exe
echo.
goto end

:clean
echo.
echo ==============================================
echo Cleaning Build Artifacts
echo ==============================================
echo.
if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
if exist "%BIN_DIR%" rmdir /s /q "%BIN_DIR%"
if exist "obj" rmdir /s /q "obj"
if exist "mylinez.exe" del /q "mylinez.exe"
if exist "mylinez_debug.exe" del /q "mylinez_debug.exe"
for /r %%i in (*.o) do del /q "%%i" >nul 2>&1
echo Clean complete!
echo.
goto end

:run
if exist "%BIN_DIR%\%TARGET%.exe" (
    cd "%BIN_DIR%"
    .\%TARGET%.exe
    cd ..
) else (
    echo Executable not found. Building first...
    call :release
    if exist "%BIN_DIR%\%TARGET%.exe" (
        cd "%BIN_DIR%"
        .\%TARGET%.exe
        cd ..
    )
)
goto end

:help
echo.
echo ==============================================
echo %TARGET% Build Script Help
echo ==============================================
echo.
echo Usage:
echo   build.bat           - Build Release version
echo   build.bat debug     - Build Debug version
echo   build.bat clean     - Clean build artifacts
echo   build.bat run       - Build and run the game
echo   build.bat help      - Show this help message
echo.
echo Build Directory: %BUILD_DIR%
echo Output Directory: %BIN_DIR%
echo Executable: %TARGET%.exe
echo.
goto end

:end
endlocal
