#include "ConvKernels.h"

std::unordered_map<Kernels, std::vector<std::vector<double>>> ConvolutionKernels::kernel_map = {
    {Kernels::GAUSSIAN_LOW_PASS, { 
        {0.0625, 0.125, 0.0625},
        {0.125, 0.25, 0.125},
        {0.0625, 0.125, 0.0625},
    }},
    
    {Kernels::LAPLACIAN_HIGH_PASS_NEGATIVE, {
        {0, 1, 0},
        {1, -4, 1},
        {0, 1, 0}
    }},
    
    {Kernels::LAPLACIAN_HIGH_PASS_POSITIVE, {     
        {0, -1, 0},
        {-1, 4, -1},
        {0, -1, 0}
    }},
    
    {Kernels::HIGH_BOOST, {
        {-1, -1, -1},
        {-1, 8, -1},
        {-1, -1, -1}
    }},
    
    {Kernels::PREWITT_HORIZONTAL, {
        {-1, 0, 1},
        {-1, 0, 1},
        {-1, 0, 1}
    }},
    
    {Kernels::PREWITT_VERTICAL, {
        {-1, -1, -1},
        {0, 0, 0},
        {1, 1, 1}
    }},
    
    {Kernels::SOBEL_HORIZONTAL, { 
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    }},
    
    {Kernels::SOBEL_VERTICAL, {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    }},

    {Kernels::CUSTOM, {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
    }}
};

