#include <opencv2/opencv.hpp>
#include <QApplication>
#include <QLabel>
#include <vector>
#include <QDebug>
#include <deque>

#include "ImageEditing.h"

// SÓ REALIZAR OPERAÇÕES AO CONVERTER A IMAGME

typedef struct Modifiers{
    cv::Mat histogram_reference_image;

    short int brightness_mod = 0;
    short int quantization = -1;
    int clockwise_rotations = 0;
    int counter_clockwise_rotations = 0;
    int times_zoomed_in = 0;
    double contrast = 1.0;
    float downScaleCoefX = 1;
    float downScaleCoefY = 1;

    Kernels kernel = NONE;

    bool is_gray_scale;
    bool is_vertically_flipped = false;
    bool is_horizontally_flipped = false;
    bool quantization_was_modified = false;
    bool contrast_was_modified = false;
    bool is_negative = false;
    bool has_histogram_equalized = false;
    bool has_histogram_matched = false;

}Modifiers;


class ImageManager{

private:    
    cv::Mat source_image;
    cv::Mat new_image;

    Modifiers previous_modifiers;
    Modifiers current_modifiers;

    Histogram histogram = ImageEditing::generateHistogram(&(this->new_image));

    int darkest_tone = 255;
    int lightest_tone = 0;
    
public:
    ImageManager(std::string source_image_path);

    void saveImage(std::string new_name);
    void renameSourceImage(std::string new_name);

    void setQuantization(uint quant);
    void setBrightnessModifier(int mod);
    void changeHorizontalFlip();
    void changeVerticalFlip();
    void setGrayScale();
    void setNegative();
    void setConvolutionKernel(Kernels kernel);
    void setConvolutionKernel(std::vector<std::vector<double>> kernel);
    void addClockwiseRotation();
    void addCounterClockwiseRotation();
    void setContrastModifier(double mod);
    void setDownscaleCoefX(float sX);
    void setDownscaleCoefY(float sY);
    void setTimesZoomedIn(int times);
    void increaseTimesZoomedIn();
    void decreaseTimesZoomedIn();
    void setHistogramEqualized();

    void generateNewImageHistogram();

    void applyChanges();


    void loadImage(std::string image_file_path);
    void reset();

    int getSourceImageQuantization();

    bool newImageIsGrayScale();
    bool sourceImageIsGrayScale();

    Histogram getNewImageHistogram();

    QImage convertSourceToQImage();
    QImage convertNewImageToQImage();

};