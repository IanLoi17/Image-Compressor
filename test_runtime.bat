@echo off
echo ===============================================
echo Testing Image Compressor Runtime
echo ===============================================
echo.

cd /d "C:\Users\jayya\Documents\GitHub\Image-Compressor"

echo Checking if executable exists...
if exist "x64\Release\ImageCompressor.exe" (
    echo [OK] Executable found
) else (
    echo [ERROR] Executable not found at x64\Release\ImageCompressor.exe
    exit /b 1
)

echo.
echo Checking if test image exists...
if exist "Images\image.jpg" (
    echo [OK] Test image found
) else (
    echo [ERROR] Test image not found at Images\image.jpg
    exit /b 1
)

echo.
echo Checking OpenCV DLL...
if exist "x64\Release\opencv_world4120.dll" (
    echo [OK] OpenCV DLL found
) else (
    echo [ERROR] OpenCV DLL not found
    exit /b 1
)

echo.
echo ===============================================
echo All runtime dependencies verified!
echo The application is ready to run.
echo ===============================================
echo.
echo To run the application:
echo   x64\Release\ImageCompressor.exe
echo.