@echo off
cd ./src

mingw32-make
if ERRORLEVEL 1 (
    echo Build failed. Press any key to continue...
    pause
) else (
	mingw32-make clean
)

@echo on
