#include "ImageEditing.h"

const double RED_COEFFICIENT = 0.299;
const double GREEN_COEFFICIENT = 0.587;
const double BLUE_COEFFICIENT = 0.114;

const int RED_VAL_INDEX = 2;
const int GREEN_CHANNEL_INDEX = 1;
const int BLUE_VAL_INDEX = 0;

void ImageEditing::changeQuantization(cv::Mat* image, uint quant){
    int cols = image->cols;
    int rows = image->rows;
    uint current_quantization;
    uint range_size;
    std::vector<uint> tone_mapping;

    current_quantization = getQuantization(*image);

    if(quant >= current_quantization){
        return;
    }

    range_size = current_quantization / quant;    
    
    for(uint i = 0; i < 255; i+= range_size){
        for(uint j = i; j < i + range_size; j++){
            tone_mapping.push_back(i + range_size);
        }
    }

    uchar new_tone;
    for(int i = 0 ; i < rows; i++){
        for(int j = 0; j < cols; j++){
            new_tone = tone_mapping[image->at<cv::Vec3b>(i, j)[0]];
            image->at<cv::Vec3b>(i, j)[0] = new_tone;
            image->at<cv::Vec3b>(i, j)[1] = new_tone;
            image->at<cv::Vec3b>(i, j)[2] = new_tone;
        }
    }
}

/*----GRAYSCALE CONVERSION ALGORITHM-----
    for every pixel with coordinates i,j
        R = pixel.red * 0.299
        G = pixel.green * 0.587
        B = pixel.blue * 0.114

        new_color = R + G + B

        pixel.red = new_color
        pixel.green = new_color
        pixel.blue = new_color
-----------------------------------------*/
void ImageEditing::convertToGrayScale(cv::Mat* image){
    int num_channels = image->channels();
    
    if(num_channels == 1) return;

    int rows = image->rows;
    int cols = image->cols;

    double R;
    double G;
    double B;
    double gray_scale_value;

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){

            R = image->at<cv::Vec3b>(i, j)[RED_VAL_INDEX] * RED_COEFFICIENT;
            G = image->at<cv::Vec3b>(i, j)[GREEN_CHANNEL_INDEX] * GREEN_COEFFICIENT;
            B = image->at<cv::Vec3b>(i, j)[BLUE_VAL_INDEX] * BLUE_COEFFICIENT;

            gray_scale_value = R + G + B;

            image->at<cv::Vec3b>(i, j)[RED_VAL_INDEX] = cv::saturate_cast<uchar>((uint) gray_scale_value);
            image->at<cv::Vec3b>(i, j)[GREEN_CHANNEL_INDEX] = cv::saturate_cast<uchar>((uint) gray_scale_value);
            image->at<cv::Vec3b>(i, j)[BLUE_VAL_INDEX] = cv::saturate_cast<uchar>((uint) gray_scale_value);
        
        }
    }
}

/*---------------VERTICAL FLIP ALGORITHM-------------
    for i = 0 to i = image.rows
        swap(image.row(i), image.row(image.rows - i))
---------------------------------------------------*/
void ImageEditing::mirrorVertical(cv::Mat* image){
    int rows = image->rows;
    int cols = image->cols * image->channels();

    uchar* temp = new uchar[cols];
    uchar* top;
    uchar* bottom;

    for(int i = 0; i < rows/2; i++){
        top = image->ptr<uchar>(i);
        bottom = image->ptr<uchar>(rows - i -1);    

        std::memcpy(temp, top, cols*sizeof(uchar));
        std::memcpy(top, bottom, cols*sizeof(uchar));
        std::memcpy(bottom, temp, cols*sizeof(uchar));
    }
        
    delete[] temp;
}

/*---HORIZONTAL FLIP ALGORITHM-------------
    for i = 0 to i = image.cols
        swap(image.col(i), image.col(image.cols - i))
-----------------------------------------*/
void ImageEditing::mirrorHorizontal(cv::Mat* image){

    int num_channels = image->channels();
    int rows = image->rows;
    int cols = image->cols * num_channels;
    
    uchar* line;
    uchar* temp = new uchar[num_channels];

    for(int i = 0; i < rows; i++){
        line = image->ptr<uchar>(i);

        for(int j = 0; j < cols/2; j+= num_channels){
            std::memcpy(temp, &line[j], num_channels);
            std::memcpy(&line[j], &line[cols - j - num_channels], num_channels);
            std::memcpy(&line[cols - j - num_channels], temp, num_channels);
        }
    }

    delete[] temp;

}

/*---BRIGHNTES ADJUSTMENT ALGORITHM---------
    for every pixel in the image
        for every channel of the pixel
            pixel[channel] += adjustment
-----------------------------------------*/
void ImageEditing::changeBrightness(cv::Mat* image, int delta){
    
    bool is_gray_scale = image->channels() == 1;
    int new_value;

    for(int i = 0; i < image->rows; i++){
        for(int j = 0; j < image->cols; j++){
            if(is_gray_scale){
                image->at<uchar>(i, j) += (uchar) delta;
            }
            else{
                for(int k = 0; k < 3; k++){
                    new_value = cv::saturate_cast<uchar>(image->at<cv::Vec3b>(i,j)[k] + delta);
                    image->at<cv::Vec3b>(i, j)[k] = new_value;
                }
            }
        }
    }
}

/*----------------------------------------
    Cv::Mat TO Qt::QImage CONVERSION
-----------------------------------------*/
QImage ImageEditing::convertMatToQImage(cv::Mat* image){
        int num_channels = image->channels();

    if(num_channels == 1){
        return QImage((uchar*)image->data, image->cols, image->rows, image->step, QImage::Format_Grayscale8);
    }
    else{
        return QImage((uchar*)image->data, image->cols, image->rows, image->step, QImage::Format_BGR888);
    }
}

/*---GRAYSCALE IMAGE QUANTIZATION FORMULA--
    lightest_tone - darkest_tone + 1;
-----------------------------------------*/
int ImageEditing::getQuantization(cv::Mat image){
    int cols = image.cols;
    int rows = image.rows;
    uint darkest_tone = 255;
    uint lightest_tone = 0;

    uint tone;
    for(int i = 0 ; i < rows; i++){
        for(int j = 0; j < cols; j++){
            tone = image.at<cv::Vec3b>(i, j)[0];
            if(tone > lightest_tone) lightest_tone = tone;
            if(tone < darkest_tone) darkest_tone = tone;
        }
    }
    return lightest_tone - darkest_tone + 1;
}


