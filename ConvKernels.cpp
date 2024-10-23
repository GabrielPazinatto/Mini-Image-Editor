#include "ConvKernels.h"

const double ConvolutionKernels::GAUSSIAN_LOW_PASS[DIM][DIM] = {
    {0.0625, 0.125, 0.0625},
    {0.125, 0.25, 0.125},
    {0.0625, 0.125, 0.0625}
};

const double ConvolutionKernels::LAPLACIAN_HIGH_PASS_NEGATIVE[DIM][DIM] = {
    {0, 1, 0},
    {1, -4, 1},
    {0, 1, 0}
};

const double ConvolutionKernels::LAPLACIAN_HIGH_PASS_POSITIVE[DIM][DIM] = {
    {0, -1, 0},
    {-1, 4, -1},
    {0, -1, 0}
};

const double ConvolutionKernels::HIGH_BOOST[DIM][DIM] = {
    {-1, -1, -1},
    {-1, 9, -1},
    {-1, -1, -1}
};

const double ConvolutionKernels::PREWITT_HORIZONTAL[DIM][DIM] = {
    {-1, 0, 1},
    {-1, 0, 1},
    {-1, 0, 1}
};

const double ConvolutionKernels::PREWITT_VERTICAL[DIM][DIM] = {
    {-1, -1, -1},
    {0, 0, 0},
    {1, 1, 1}
};