#include <opencv2/opencv.hpp>
#include <QApplication>
#include <QLabel>
#include <vector>
#include <QDebug>
#include <deque>

#include "ImageEditing.h"

// SÓ REALIZAR OPERAÇÕES AO CONVERTER A IMAGME

typedef struct Modifiers{
    short int brightness_mod = 0;
    short int quantization = -1;
    
    bool is_gray_scale;
    bool is_vertically_flipped = false;
    bool is_horizontally_flipped = false;
    bool quantization_was_modified = false;
}Modifiers;


class ImageManager{

private:    
    cv::Mat source_image;
    cv::Mat new_image;

    Modifiers previous_modifiers;
    Modifiers current_modifiers;

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

    void applyChanges();

    void loadImage(std::string image_file_path);
    void reset();

    int getSourceImageQuantization();

    bool newImageIsGrayScale();
    bool sourceImageIsGrayScale();

    QImage convertSourceToQImage();
    QImage convertNewImageToQImage();

};