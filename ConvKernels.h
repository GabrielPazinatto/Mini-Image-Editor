#define DIM 3

class ConvolutionKernels{

public:
    static const double GAUSSIAN_LOW_PASS[DIM][DIM];
    static const double LAPLACIAN_HIGH_PASS_NEGATIVE[DIM][DIM];
    static const double LAPLACIAN_HIGH_PASS_POSITIVE[DIM][DIM];
    static const double HIGH_BOOST[DIM][DIM];
    static const double PREWITT_HORIZONTAL[DIM][DIM];
    static const double PREWITT_VERTICAL[DIM][DIM];
};

