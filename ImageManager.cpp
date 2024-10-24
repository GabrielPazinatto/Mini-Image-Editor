#include "ImageManager.h"

const double RED_COEFFICIENT = 0.299;
const double GREEN_COEFFICIENT = 0.587;
const double BLUE_COEFFICIENT = 0.114;

const int RED_CHANNEL_INDEX = 2;
const int GREEN_CHANNEL_INDEX = 1;
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

// EFFECTIVELY applies the changes to the new image
void ImageManager::applyChanges(){
    this->new_image = this->source_image.clone();

    if(current_modifiers.quantization_was_modified){
        ImageEditing::changeQuantization(&(this->new_image), this->current_modifiers.quantization);
    }

    if(current_modifiers.brightness_mod != 0){
        ImageEditing::changeBrightness(&(this->new_image), current_modifiers.brightness_mod);
    } 

    if(current_modifiers.is_negative){
        ImageEditing::convertToNegative(&(this->new_image));
    }   

    if(current_modifiers.is_gray_scale){
        ImageEditing::convertToGrayScale(&(this->new_image));
    }

    if(current_modifiers.is_horizontally_flipped){
        ImageEditing::mirrorHorizontal(&(this->new_image));
    }

    if(current_modifiers.is_vertically_flipped){
        ImageEditing::mirrorVertical(&(this->new_image));
    }   
}


