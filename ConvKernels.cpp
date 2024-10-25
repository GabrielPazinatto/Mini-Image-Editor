#include "ConvKernels.h"

const std::vector<std::vector<double>>& ConvolutionKernels::GAUSSIAN_LOW_PASS = {
    {0.0625, 0.125, 0.0625},
    {0.125, 0.25, 0.125},
    {0.0625, 0.125, 0.0625}
};

const std::vector<std::vector<double>>&ConvolutionKernels::LAPLACIAN_HIGH_PASS_NEGATIVE = {
    {0, 1, 0},
    {1, -4, 1},
    {0, 1, 0}
};

const std::vector<std::vector<double>>& ConvolutionKernels::LAPLACIAN_HIGH_PASS_POSITIVE = {
    {0, -1, 0},
    {-1, 4, -1},
    {0, -1, 0}
};

const std::vector<std::vector<double>>&ConvolutionKernels::HIGH_BOOST = {
    {-1, -1, -1},
    {-1, 8, -1},
    {-1, -1, -1}
};

const std::vector<std::vector<double>>& ConvolutionKernels::PREWITT_HORIZONTAL = {
    {-1, 0, 1},
    {-1, 0, 1},
    {-1, 0, 1}
};

const std::vector<std::vector<double>>& ConvolutionKernels::PREWITT_VERTICAL = {
    {-1, -1, -1},
    {0, 0, 0},
    {1, 1, 1}
};  

const std::vector<std::vector<double>>&ConvolutionKernels::SOBEL_HORIZONTAL = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

const std::vector<std::vector<double>>& ConvolutionKernels::SOBEL_VERTICAL = {
    {-1, -2, -1},
    {0, 0, 0},
    {1, 2, 1}
};