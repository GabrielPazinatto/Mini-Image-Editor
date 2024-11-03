#include "ImageManager.h"

const double RED_COEFFICIENT = 0.299;
const double GREEN_COEFFICIENT = 0.587;
const double BLUE_COEFFICIENT = 0.114;

const int RED_CHANNEL_INDEX = 2;
const int GREEN_CHANNEL = 1;
const int BLUE_CHANNEL_INDEX = 0;

ImageManager::ImageManager(std::string source_image_path = ""){
    if(source_image_path == "") return;
    this->loadImage(source_image_path);
    this->previous_modifiers.is_gray_scale = this->source_image.channels() == 1;
}

void ImageManager::loadImage(std::string image_file_path){
    this->source_image = cv::imread(image_file_path);
    
    if(this->source_image.empty()) return;

    this->new_image = this->source_image.clone();

    this->previous_modifiers.is_gray_scale = this->source_image.channels() == 1;
    this->previous_modifiers.quantization = ImageEditing::getQuantization(this->source_image);
    this->current_modifiers = this->previous_modifiers;
}

void ImageManager::reset(){
    this->new_image = this->source_image.clone();
    this->current_modifiers = this->previous_modifiers;
}

Histogram ImageManager::getNewImageHistogram(){
    return this->histogram;
}


void ImageManager::generateNewImageHistogram(){
    this->histogram = ImageEditing::generateHistogram(&(this->new_image));
}

void ImageManager::setNegative(){
    this->current_modifiers.is_negative = !this->current_modifiers.is_negative;
}

// saves the source image with a new name
void ImageManager::renameSourceImage(std::string new_name){
    cv::imwrite(new_name, this->source_image);
}

// saves the modified new image
void ImageManager::saveImage(std::string new_name){
    cv::imwrite(new_name, this->new_image);
}

void ImageManager::setConvolutionKernel(Kernels kernel){
    this->current_modifiers.kernel = kernel;
}

void ImageManager::setConvolutionKernel(std::vector<std::vector<double>> kernel){
    for(int i = 0 ; i < 3; i++){
        for(int j = 0; j < 3; j++){
            ConvolutionKernels::kernel_map[Kernels::CUSTOM][i][j] = kernel[i][j];
        }
    }
    this->current_modifiers.kernel = Kernels::CUSTOM;
}

void ImageManager::setDownscaleCoefX(float sX){
    this->current_modifiers.downScaleCoefX = sX;
}

void ImageManager::setDownscaleCoefY(float sY){
    this->current_modifiers.downScaleCoefY = sY;
}


void ImageManager::addClockwiseRotation(){
    this->current_modifiers.clockwise_rotations++;
}

void ImageManager::addCounterClockwiseRotation(){
    this->current_modifiers.counter_clockwise_rotations++;
}

// stores the value for the quantization value
void ImageManager::setQuantization(uint quant){
    this->current_modifiers.quantization_was_modified = true;
    this->current_modifiers.quantization = quant;
}

// stores the value for the brightness modifier value
void ImageManager::setBrightnessModifier(int mod){
    this->current_modifiers.brightness_mod = mod;
}

// changes the flag to make a horizontal flip
void ImageManager::changeHorizontalFlip(){
    this->current_modifiers.is_horizontally_flipped = !this->current_modifiers.is_horizontally_flipped;
}

// changes the flag to make a vertical flip
void ImageManager::changeVerticalFlip(){
    this->current_modifiers.is_vertically_flipped = !this->current_modifiers.is_vertically_flipped;
}

// changes the flag to convert the image to gray scale
void ImageManager::setGrayScale(){
    this->current_modifiers.is_gray_scale = true;
}

// sets the times the image has been zoomed in
void ImageManager::setTimesZoomedIn(int times){
    this->current_modifiers.times_zoomed_in = times;
}

void ImageManager::decreaseTimesZoomedIn(){
    if(this->current_modifiers.times_zoomed_in != 0){
        this->current_modifiers.times_zoomed_in--;
    }
}

// increases the times the image has been zoomed in
void ImageManager::increaseTimesZoomedIn(){
    this->current_modifiers.times_zoomed_in++;
}

// changes the contrast modifier
void ImageManager::setContrastModifier(double mod){ 
    if(mod > 0 && mod <= 255){
        this->current_modifiers.contrast = mod;
        this->current_modifiers.contrast_was_modified = true;
    }
}

void ImageManager::setHistogramEqualized(){
    this->current_modifiers.has_histogram_equalized = !this->current_modifiers.has_histogram_equalized;
}

void ImageManager::setHistogramEqualized(bool equalized){
    this->current_modifiers.has_histogram_equalized = equalized;
}

void ImageManager::setHistogramMatched(bool matched, cv::Mat reference_image){
    this->current_modifiers.has_histogram_matched = matched;
    this->current_modifiers.histogram_reference_image = reference_image;
}

// returns a QImage copy of the source image
QImage ImageManager::convertSourceToQImage(){
    return ImageEditing::convertMatToQImage(&(this->source_image));
}

// returns a QImage copy of the edited image
QImage ImageManager::convertNewImageToQImage(){
    return ImageEditing::convertMatToQImage(&(this->new_image));
}

// returns the quantization of the source image
int ImageManager::getSourceImageQuantization(){
    return ImageEditing::getQuantization(this->new_image);
}

// tells if the new image is or has been converted to gray scale
bool ImageManager::newImageIsGrayScale(){
    return this->current_modifiers.is_gray_scale;
}

// tells if the source image is or has been converted to gray scale
bool ImageManager::sourceImageIsGrayScale(){
    return this->previous_modifiers.is_gray_scale;
}

cv::Mat* ImageManager::getNewImage(){
    return &(this->new_image);
}


cv::Mat ImageManager::getHistogramEqReferenceImage(){
    return this->current_modifiers.histogram_reference_image;
}

// EFFECTIVELY applies the changes to the new image
void ImageManager::applyChanges(){
    this->new_image = this->source_image.clone();

    if(current_modifiers.is_gray_scale){
        ImageEditing::convertToGrayScale(&(this->new_image));
    }

    if(current_modifiers.has_histogram_equalized){
        ImageEditing::equalizeHistogram(&(this->new_image), this->current_modifiers.is_gray_scale);
    }

    if(current_modifiers.has_histogram_matched){
        ImageEditing::matchHistogram(&(this->new_image), &(this->current_modifiers.histogram_reference_image));
    }

    if(current_modifiers.quantization_was_modified){
        ImageEditing::changeQuantization(&(this->new_image), this->current_modifiers.quantization);
    }

    if(current_modifiers.brightness_mod != 0){
        ImageEditing::changeBrightness(&(this->new_image), current_modifiers.brightness_mod);
    } 

    if(current_modifiers.is_negative){
        ImageEditing::convertToNegative(&(this->new_image));
    }   

    if(current_modifiers.is_horizontally_flipped){
        ImageEditing::mirrorHorizontal(&(this->new_image));
    }

    if(current_modifiers.kernel != Kernels::NONE){
        ImageEditing::applyConvolution(&(this->new_image), ConvolutionKernels::kernel_map.at(current_modifiers.kernel));
    }

    if(current_modifiers.is_vertically_flipped){
        ImageEditing::mirrorVertical(&(this->new_image));
    }

    for(int i = 0; i < current_modifiers.clockwise_rotations % 4; i++){
        ImageEditing::rotateImage(&(this->new_image), true);
    }

    for(int i = 0; i < current_modifiers.counter_clockwise_rotations % 4; i++){
        ImageEditing::rotateImage(&(this->new_image), false);
    }

    if(current_modifiers.contrast_was_modified == true){
        ImageEditing::changeContrast(&(this->new_image), current_modifiers.contrast);
    }

    if(current_modifiers.downScaleCoefX != 1 || current_modifiers.downScaleCoefY != 1){
        ImageEditing::zoomOut(&(this->new_image), current_modifiers.downScaleCoefX, current_modifiers.downScaleCoefY);
    }

    for(int i = 0; i< current_modifiers.times_zoomed_in; i++){
        ImageEditing::zoomIn(&(this->new_image));
    }

}


