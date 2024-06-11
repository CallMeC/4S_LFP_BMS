#include "systemCore.h"
#include "appVariable.h"

float c_SystemCore::linearInterpolation(float x0, float x1, float y0, float y1, float x)
{
    return y0 + ((y1 - y0) * (x - x0) / (x1 - x0));
}

float c_SystemCore::getInterpolatedValue(const std::vector<float>& matrix, int numRows, int numCols, float x, float y)
{
    // Extract X and Y vectors from the first row and column of the matrix
    std::vector<float> xValues(numCols - 1);
    std::vector<float> yValues(numRows - 1);

    for (int i = 0; i < numCols - 1; ++i) {
        xValues[i] = matrix[i + 1];
    }
    for (int i = 0; i < numRows - 1; ++i) {
        yValues[i] = matrix[(numCols) * (i + 1)];
    }

    // Find indices for X
    int xIndex = 0;
    while (xIndex < numCols - 2 && x > xValues[xIndex + 1]) {
        xIndex++;
    }

    // Find indices for Y
    int yIndex = 0;
    while (yIndex < numRows - 2 && y > yValues[yIndex + 1]) {
        yIndex++;
    }

    // Values at the corners of the cell in the matrix
    float x0 = xValues[xIndex];
    float x1 = xValues[xIndex + 1];
    float y0 = yValues[yIndex];
    float y1 = yValues[yIndex + 1];

    float z00 = matrix[(yIndex + 1) * numCols + xIndex + 1];
    float z01 = matrix[(yIndex + 1) * numCols + xIndex + 2];
    float z10 = matrix[(yIndex + 2) * numCols + xIndex + 1];
    float z11 = matrix[(yIndex + 2) * numCols + xIndex + 2];

    // Linear interpolation in the X direction for each Y
    float z0 = linearInterpolation(x0, x1, z00, z01, x);
    float z1 = linearInterpolation(x0, x1, z10, z11, x);

    // Linear interpolation in the Y direction
    float z = linearInterpolation(y0, y1, z0, z1, y);

    return z;
}