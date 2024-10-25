#include <opencv2/opencv.hpp>
#include <QImage>
#include <thread>
#include <vector>

#include "omp.h"
#include "ConvKernels.h"

namespace ImageEditing{
    void changeBrightness(cv::Mat* image, int delta);
    void convertToGrayScale(cv::Mat* image);
    void mirrorVertical(cv::Mat* image);
    void mirrorHorizontal(cv::Mat* image);
    void changeQuantization(cv::Mat* image, uint quant);
    void convertToNegative(cv::Mat* image);
    void applyConvolution(cv::Mat* image, const std::vector<std::vector<double>> kernel);
    

    QImage convertMatToQImage(cv::Mat* image);
    int getQuantization(cv::Mat image);}