#ifndef __SYSTEM_CORE__
#define __SYSTEM_CORE__

#include "pico/stdlib.h"
#include <stdio.h>

#include <iostream>
#include <vector>

class c_SystemCore
{
    public:
        float linearInterpolation(float x0, float x1, float y0, float y1, float x);
        float getInterpolatedValue(const std::vector<float>& matrix, int numRows, int numCols, float x, float y);
};

#endif