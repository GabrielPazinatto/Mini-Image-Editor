#ifndef CONVKERNELS_H
#define CONVKERNELS_H

#include <unordered_map>
#include <vector>

#define DIM 3

enum Kernels{
    GAUSSIAN_LOW_PASS,
    LAPLACIAN_HIGH_PASS_NEGATIVE,
    LAPLACIAN_HIGH_PASS_POSITIVE,
    HIGH_BOOST,
    PREWITT_HORIZONTAL,
    PREWITT_VERTICAL,
    SOBEL_HORIZONTAL,
    SOBEL_VERTICAL,
    NONE,
    CUSTOM
};

class ConvolutionKernels{

public:
    static const std::unordered_map<Kernels, std::vector<std::vector<double>>> kernel_map;
};

#endif