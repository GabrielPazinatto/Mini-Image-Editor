#include "Histogram.h"


const double RED_COEFFICIENT = 0.299;
const double GREEN_COEFFICIENT = 0.587;
const double BLUE_COEFFICIENT = 0.114;

const int RED_CHANNEL_INDEX = 2;
const int GREEN_CHANNEL = 1;
const int BLUE_CHANNEL_INDEX = 0;

Histogram::Histogram(cv::Mat* image){
    for(int i = 0; i < TONE_RANGE; i++){
        this->red_channel.push_back(0);
        this->green_channel.push_back(0);
        this->blue_channel.push_back(0);
        this->luminance_channel.push_back(0);
    }

    if(image->empty()) return;

    int cols = image->cols;
    int rows = image->rows;

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){

            this->red_channel[image->at<cv::Vec3b>(i, j)[RED_CHANNEL_INDEX]]++;
            this->green_channel[image->at<cv::Vec3b>(i, j)[GREEN_CHANNEL]]++;
            this->blue_channel[image->at<cv::Vec3b>(i, j)[BLUE_CHANNEL_INDEX]]++;

            this->luminance_channel[(int)(RED_COEFFICIENT * image->at<cv::Vec3b>(i, j)[RED_CHANNEL_INDEX] + 
                                        GREEN_COEFFICIENT * image->at<cv::Vec3b>(i, j)[GREEN_CHANNEL] + 
                                        BLUE_COEFFICIENT * image->at<cv::Vec3b>(i, j)[BLUE_CHANNEL_INDEX])]++;
        }
    }
}


std::vector<double>& Histogram::getRedChannel(){
    return this->red_channel;
}

std::vector<double>& Histogram::getGreenChannel(){
    return this->green_channel;
}

std::vector<double>& Histogram::getBlueChannel(){
    return this->blue_channel;
}

std::vector<double>& Histogram::getLuminanceChannel(){
    return this->luminance_channel;
}
