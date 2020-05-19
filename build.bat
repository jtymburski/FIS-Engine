::==============================================================================
:: Description: Runs the build of Univursa with the necessary parameters for 
::              MinGW. 
:: Date: November 16, 2013
::==============================================================================
@echo off

:: Create output directory if it doesn't exist
:: if not exist bin mkdir bin

:: Attempt to build the application,outputting result to terminal as well
:: as output file "build-stdout.log" and the error/warning information to
:: "build-stderr.log"
echo.
echo --------------------- BUILD SEQUENCE ---------------------
mingw32-make -j8 2> build-stderr.log | tee build-stdout.log
echo ----------------------------------------------------------
echo.
echo -------------- ERROR/WARNING NOTIFICATIONS ---------------
cat build-stderr.log
echo ----------------------------------------------------------
echo.

:: Ask the user if they want to run the application
set /p choice="Do you want to run Univursa.exe? " %=%
if %choice%==Y start run.bat
if %choice%==y start run.bat

:: Pause and wait for user to close
pause

