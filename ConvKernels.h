#ifndef CONVKERNELS_H
#define CONVKERNELS_H

#include <map>
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
    NONE
};

class ConvolutionKernels{

private:
    static const std::vector<std::vector<double>>& GAUSSIAN_LOW_PASS;
    static const std::vector<std::vector<double>>& LAPLACIAN_HIGH_PASS_NEGATIVE;
    static const std::vector<std::vector<double>>& LAPLACIAN_HIGH_PASS_POSITIVE;
    static const std::vector<std::vector<double>>& HIGH_BOOST;
    static const std::vector<std::vector<double>>& PREWITT_HORIZONTAL;
    static const std::vector<std::vector<double>>& PREWITT_VERTICAL;
    static const std::vector<std::vector<double>>& SOBEL_HORIZONTAL;
    static const std::vector<std::vector<double>>& SOBEL_VERTICAL;
public:
    const std::unordered_map<Kernels, std::vector<std::vector<double>>>& kernel_map = {
        {Kernels::GAUSSIAN_LOW_PASS, GAUSSIAN_LOW_PASS},
        {Kernels::LAPLACIAN_HIGH_PASS_NEGATIVE, LAPLACIAN_HIGH_PASS_NEGATIVE},
        {Kernels::LAPLACIAN_HIGH_PASS_POSITIVE, LAPLACIAN_HIGH_PASS_POSITIVE},
        {Kernels::HIGH_BOOST, HIGH_BOOST},
        {Kernels::PREWITT_HORIZONTAL, PREWITT_HORIZONTAL},
        {Kernels::PREWITT_VERTICAL, PREWITT_VERTICAL},
        {Kernels::SOBEL_HORIZONTAL, SOBEL_HORIZONTAL},
        {Kernels::SOBEL_VERTICAL, SOBEL_VERTICAL}
    };
};

#endif