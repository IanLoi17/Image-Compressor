@echo off
setlocal enabledelayedexpansion

echo ===============================================
echo Image Compressor Build Script
echo ===============================================
echo.

:: Set OpenCV version and paths
set OPENCV_VERSION=4.12.0
set OPENCV_DIR=C:\opencv
set OPENCV_BUILD_DIR=%OPENCV_DIR%\build
set OPENCV_DLL_DIR=%OPENCV_BUILD_DIR%\x64\vc16\bin
set OPENCV_DOWNLOAD_URL=https://github.com/opencv/opencv/releases/download/%OPENCV_VERSION%/opencv-%OPENCV_VERSION%-windows.exe

:: Set project output directories
set PROJECT_DIR=%~dp0
set X64_DEBUG_DIR=%PROJECT_DIR%x64\Debug
set X64_RELEASE_DIR=%PROJECT_DIR%x64\Release

:: Check if OpenCV is installed
echo Checking for OpenCV installation at %OPENCV_DIR%...
if exist "%OPENCV_BUILD_DIR%" (
    echo [OK] OpenCV found at %OPENCV_DIR%
) else (
    echo [WARNING] OpenCV not found!
    echo.
    echo Downloading OpenCV %OPENCV_VERSION%...
    echo This may take several minutes...
    echo.

    :: Create temp directory for download
    set TEMP_DIR=%TEMP%\opencv_download
    if not exist "!TEMP_DIR!" mkdir "!TEMP_DIR!"

    :: Download OpenCV using PowerShell script
    powershell -ExecutionPolicy Bypass -File "%PROJECT_DIR%download_opencv.ps1" -DownloadUrl "%OPENCV_DOWNLOAD_URL%" -OutputPath "!TEMP_DIR!\opencv.exe"

    if errorlevel 1 (
        echo [ERROR] Failed to download OpenCV!
        echo Please download manually from: %OPENCV_DOWNLOAD_URL%
        echo Extract to: C:\opencv
        pause
        exit /b 1
    )

    echo.
    echo Extracting OpenCV to C:\...
    echo This is a self-extracting archive. Please extract to C:\ when prompted.
    echo.
    pause

    :: Run the self-extracting archive
    "!TEMP_DIR!\opencv.exe" -o"C:\" -y

    if not exist "%OPENCV_BUILD_DIR%" (
        echo [ERROR] OpenCV extraction failed!
        echo Please manually extract opencv.exe to C:\
        pause
        exit /b 1
    )

    echo [OK] OpenCV installed successfully!

    :: Cleanup
    rd /s /q "!TEMP_DIR!"
)

echo.
echo ===============================================
echo Copying OpenCV DLLs to output directories
echo ===============================================
echo.

:: Create output directories if they don't exist
if not exist "%X64_DEBUG_DIR%" (
    echo Creating directory: %X64_DEBUG_DIR%
    mkdir "%X64_DEBUG_DIR%"
)

if not exist "%X64_RELEASE_DIR%" (
    echo Creating directory: %X64_RELEASE_DIR%
    mkdir "%X64_RELEASE_DIR%"
)

:: Copy Debug DLL
if exist "%OPENCV_DLL_DIR%\opencv_world4120d.dll" (
    echo Copying opencv_world4120d.dll to %X64_DEBUG_DIR%...
    copy /Y "%OPENCV_DLL_DIR%\opencv_world4120d.dll" "%X64_DEBUG_DIR%\" >nul
    if errorlevel 1 (
        echo [ERROR] Failed to copy Debug DLL
    ) else (
        echo [OK] Debug DLL copied successfully
    )
) else (
    echo [WARNING] Debug DLL not found at %OPENCV_DLL_DIR%
)

:: Copy Release DLL
if exist "%OPENCV_DLL_DIR%\opencv_world4120.dll" (
    echo Copying opencv_world4120.dll to %X64_RELEASE_DIR%...
    copy /Y "%OPENCV_DLL_DIR%\opencv_world4120.dll" "%X64_RELEASE_DIR%\" >nul
    if errorlevel 1 (
        echo [ERROR] Failed to copy Release DLL
    ) else (
        echo [OK] Release DLL copied successfully
    )
) else (
    echo [WARNING] Release DLL not found at %OPENCV_DLL_DIR%
)

echo.
echo ===============================================
echo Build Setup Complete!
echo ===============================================
echo.
echo You can now build the project using:
echo   - Visual Studio (Open ImageCompressor.sln)
echo   - MSBuild from command line
echo.
echo Output directories:
echo   Debug:   %X64_DEBUG_DIR%
echo   Release: %X64_RELEASE_DIR%
echo.

endlocal
pause
