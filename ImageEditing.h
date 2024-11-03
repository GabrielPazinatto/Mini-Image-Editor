#include <opencv2/opencv.hpp>
#include <QImage>
#include <thread>
#include <vector>

#include "omp.h"
#include "ConvKernels.h"
#include "Histogram.h"

namespace ImageEditing{
    void changeBrightness(cv::Mat* image, int delta);
    void convertToGrayScale(cv::Mat* image);
    void mirrorVertical(cv::Mat* image);
    void mirrorHorizontal(cv::Mat* image);
    void changeQuantization(cv::Mat* image, uint quant);
    void convertToNegative(cv::Mat* image);
    void applyConvolution(cv::Mat* image, const std::vector<std::vector<double>> kernel, bool sum_127 = true);
    void rotateImage(cv::Mat* image, bool clockwise);
    void changeContrast(cv::Mat* image, double modifier);
    void zoomOut(cv::Mat* image, float sX, float sY);
    void zoomIn(cv::Mat* image);
    Histogram generateHistogram(const cv::Mat* image);
    Histogram generateCumulativeHistogram(const cv::Mat* image);
    void matchHistogram(cv::Mat* image, const cv::Mat* reference);
    void equalizeHistogram(cv::Mat* image, bool image_is_grayscale = false);

    QImage convertMatToQImage(cv::Mat* image);
    int getQuantization(cv::Mat image);}