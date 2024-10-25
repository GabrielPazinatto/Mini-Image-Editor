#ifndef CONVKERNELS_H
#define CONVKERNELS_H

#define DIM 3
#include <map>

class ConvolutionKernels{

public:
    static const double GAUSSIAN_LOW_PASS[DIM][DIM];
    static const double LAPLACIAN_HIGH_PASS_NEGATIVE[DIM][DIM];
    static const double LAPLACIAN_HIGH_PASS_POSITIVE[DIM][DIM];
    static const double HIGH_BOOST[DIM][DIM];
    static const double PREWITT_HORIZONTAL[DIM][DIM];
    static const double PREWITT_VERTICAL[DIM][DIM];
    static const double SOBEL_HORIZONTAL[DIM][DIM];
    static const double SOBEL_VERTICAL[DIM][DIM];
    
};

#endif