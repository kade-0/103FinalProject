@echo off
setlocal enabledelayedexpansion

mkdir bin 2>nul || rem Silently continue if bin already exists
cd src

set files=
for %%f in (*.cpp) do (
    set files=!files! "%%f"
)

echo Compiling files: %files%
g++ -o ../bin/final-project.exe %files%

if %errorlevel% equ 0 (
    cd ../bin/
    echo Running program...
    echo -------------------
    final-project.exe
) else (
    echo Compilation failed with error %errorlevel%
    exit /b %errorlevel%
)