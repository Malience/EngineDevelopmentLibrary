@echo off

REM Check for admin rights
net session >nul 2>&1
if not %errorLevel% == 0 (
	echo FAILED: Current permissions insufficient!
	echo FAILED: Administrator privileges are required for installation!
	PAUSE
	exit /b 1
)

REM Check if the project has been built yet
if not exist %~dp0\build (
	echo FAILED: Project is not built!
	echo FAILED: The project need to be built before installation!
	PAUSE
	exit /b 1
)

cd %~dp0\build
cmake --build . --target install --config Debug
cmake --build . --target install --config Release
PAUSE