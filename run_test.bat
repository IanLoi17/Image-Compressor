@echo off
echo ===============================================
echo Starting Image Compressor
echo ===============================================
echo.
echo This will launch the Image Compressor GUI.
echo You can test the following features:
echo.
echo 1. Dynamic Programming vs Greedy Comparison
echo    - Click "Generate Start" to compare algorithms
echo.
echo 2. Visualize Seam Carving
echo    - Click "Shrink Width" buttons to reduce width
echo    - Click "Shrink Height" buttons to reduce height
echo    - Use x10 or x100 for batch processing
echo.
echo 3. Graph Cut (Forward Energy) Mode
echo    - Click "GC Width" buttons for forward energy seams
echo.
echo 4. Reset button to restore original image
echo.
echo Press Ctrl+C to stop the application
echo ===============================================
echo.

cd /d "C:\Users\jayya\Documents\GitHub\Image-Compressor"
"x64\Release\ImageCompressor.exe"