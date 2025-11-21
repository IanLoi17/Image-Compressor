# Graph Cut (Forward Energy) Functionality Explained

## How It Works

YES, the Graph Cut buttons **DO compress the image** by removing seams!

### What Happens When You Click Graph Cut Buttons:

1. **"GC Width x1"** - Removes 1 vertical seam (reduces width by 1 pixel)
2. **"GC x10"** - Removes 10 vertical seams (reduces width by 10 pixels)
3. **"GC x100"** - Removes 100 vertical seams (reduces width by 100 pixels)
4. **"GC Height x1"** - Removes 1 horizontal seam (reduces height by 1 pixel)
5. **"GC x10"** - Removes 10 horizontal seams (reduces height by 10 pixels)

### Visual Feedback:

- The last seam removed is shown in **GREEN** color
- Regular DP seams are shown in **RED** color
- This helps you see which pixels are being removed

### The Process:

1. **Calculates Forward Energy**: Uses an improved energy function that considers the energy introduced by new edges after seam removal
2. **Finds Optimal Seam**: Uses dynamic programming to find the minimum energy path
3. **Removes the Seam**: Actually deletes those pixels from the image
4. **Updates Display**: Shows the compressed image with the last seam highlighted

### Key Differences from Regular Seam Carving:

- **Regular (DP)**: Uses gradient magnitude energy
- **Graph Cut (Forward Energy)**: Uses forward energy that minimizes artifacts
- **Result**: Graph Cut typically produces better quality compressed images with fewer artifacts

### Code Flow:

```cpp
// When you click "GC Width x1":
carveSeamOnceAndShowGraphCut(true)
  -> findVerticalSeamGraphCut()    // Finds the seam
  -> removeSeam(seam, true)        // REMOVES the seam (compresses image)
  -> returns visualization          // Shows the removed seam in green
```

## Testing the Compression:

1. Run the application: `x64\Release\ImageCompressor.exe`
2. Look at the "Current Dimensions" display
3. Click any Graph Cut button
4. Watch the dimensions decrease (image is compressed!)
5. The visualization window shows the compressed result

## Note:

Despite the name "Graph Cut", this implementation actually uses **Forward Energy Dynamic Programming**, not true graph cuts (max-flow/min-cut). The naming is misleading but the compression functionality works correctly!