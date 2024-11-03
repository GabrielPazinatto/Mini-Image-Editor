#include "ImageEditing.h"

const double RED_COEFFICIENT = 0.299;
const double GREEN_COEFFICIENT = 0.587;
const double BLUE_COEFFICIENT = 0.114;

const int RED_CHANNEL = 2;
const int GREEN_CHANNEL = 1;
const int BLUE_CHANNEL = 0;

/*-------------------------------------
             ZOOMING
-------------------------------------*/

void ImageEditing::zoomIn(cv::Mat* image){
    int cols = image->cols;
    int rows = image->rows;
    int channels = image->channels();
    double new_value;

    cv::Mat new_image = cv::Mat::zeros(rows*2, cols*2, image->type());

    for(int i = 0 ; i < rows*2; i+=2){
        for(int j = 0; j < cols*2; j+=2){
            for(int c = 0; c < channels; c++){
                new_image.at<cv::Vec3b>(i, j)[c] = image->at<cv::Vec3b>(i/2, j/2)[c];
            }
        }
    }

    for (int i = 0; i < rows * 2; i += 2) {
        for (int j = 1; j < cols * 2 - 1; j += 2) {
            for (int c = 0; c < channels; c++) {
                double left_value = new_image.at<cv::Vec3b>(i, j - 1)[c];
                double right_value = new_image.at<cv::Vec3b>(i, j + 1)[c];
                new_image.at<cv::Vec3b>(i, j)[c] = cv::saturate_cast<uchar>((left_value + right_value) / 2.0);
            }
        }
    }

    for (int i = 1; i < rows * 2 - 1; i += 2) {
        for (int j = 0; j < cols * 2; j++) {
            for (int c = 0; c < channels; c++) {
                double top_value = new_image.at<cv::Vec3b>(i - 1, j)[c];
                double bottom_value = new_image.at<cv::Vec3b>(i + 1, j)[c];
                new_image.at<cv::Vec3b>(i, j)[c] = cv::saturate_cast<uchar>((top_value + bottom_value) / 2.0);
            }
        }
    }

    for (int i = 1; i < rows * 2 - 1; i += 2) {
        for (int j = 1; j < cols * 2 - 1; j += 2) {
            for (int c = 0; c < channels; c++) {
                double top_left = new_image.at<cv::Vec3b>(i - 1, j - 1)[c];
                double top_right = new_image.at<cv::Vec3b>(i - 1, j + 1)[c];
                double bottom_left = new_image.at<cv::Vec3b>(i + 1, j - 1)[c];
                double bottom_right = new_image.at<cv::Vec3b>(i + 1, j + 1)[c];
                new_image.at<cv::Vec3b>(i, j)[c] = cv::saturate_cast<uchar>((top_left + top_right + bottom_left + bottom_right) / 4.0);
            }
        }
    }

    *image = new_image.clone();
}


void ImageEditing::zoomOut(cv::Mat* image, float sX, float sY){
    if(sX < 1 || sY < 1) return;
    
    int cols = image->cols;
    int rows = image->rows;
    int channels = image->channels();
    std::array<double, 3> pixel;

    cv::Mat new_image = cv::Mat::zeros(rows/sY, cols/sX, image->type());

    for(int i = 0 ; i < rows/sY; i++){              
        for(int j = 0; j < cols/sX; j++){
            pixel = {0, 0, 0};
            for(int k = 0; k < sX; k++){
                for(int l = 0; l < sY; l++){
                    for(int c = 0; c < channels; c++){
                        if(i*sY + l >= rows || j*sX + k >= cols) continue;
                        pixel[c] += image->at<cv::Vec3b>(i*sY + l, j*sX + k)[c];
                    }
                }
            }
        
        for(int c = 0; c < channels; c++){
            if(i >= new_image.rows || j >= new_image.cols) continue;
            new_image.at<cv::Vec3b>(i, j)[c] = cv::saturate_cast<uchar>(pixel[c] / (sX * sY));
        }

        }
    }
    *image = new_image.clone();
}

/*-------------------------------------
            ROTATIONS
-------------------------------------*/

void ImageEditing::rotateImage(cv::Mat* image, bool clockwise = true){
    int cols = image->cols;
    int rows = image->rows;

    cv::Mat new_image;
    
    
    if(clockwise){
        new_image.create(cols, rows, image->type());
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                new_image.at<cv::Vec3b>(j, rows - i - 1) = image->at<cv::Vec3b>(i, j);
            }
        }
    }

    else{
        new_image.create(cols, rows, image->type());
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                new_image.at<cv::Vec3b>(cols - j - 1, i) = image->at<cv::Vec3b>(i, j);
            }
        }
    }

    *image = new_image.clone();
}

/*-------------------------------------
            CONVOLUTIONS
-------------------------------------*/

// performs a 180 counter-clockwise rotation
// very weird code, but it works
void rotateKernel(std::vector<std::vector<double>>& kernel){
    std::swap(kernel[0][0], kernel[0][2]);
    std::swap(kernel[1][0], kernel[1][2]);
    std::swap(kernel[2][0], kernel[2][2]);
    
    for(int i = 0; i < DIM; i++){
        std::swap(kernel[0][i], kernel[2][i]);
    }
}

void applyConvolutionToPoint(const cv::Mat* source_image, cv::Mat* new_image, int row, int col, const std::vector<std::vector<double>> kernel){
    double R = 0; 
    double G = 0; 
    double B = 0;
    double L = 0;

    #pragma omp parallel for collapse(2)
    for(int i = -((int)DIM/2); i < (int) DIM/2 + 1; i++){
        for(int j = -((int)DIM/2); j < (int) DIM/2 + 1; j++){

            if(source_image->channels() == 3){
                    R += kernel[i+DIM/2][j + DIM/2] * source_image->at<cv::Vec3b>(row + i, col + j)[RED_CHANNEL];
                    G += kernel[i+DIM/2][j + DIM/2] * source_image->at<cv::Vec3b>(row + i, col + j)[GREEN_CHANNEL];
                    B += kernel[i+DIM/2][j + DIM/2] * source_image->at<cv::Vec3b>(row + i, col + j)[BLUE_CHANNEL];
            }

            else if(source_image->channels() == 1){
                L += kernel[i][j] * source_image->at<double>(row + i, col + j);
            }
        }
    }

    if(source_image->channels() == 3){
        new_image->at<cv::Vec3b>(row, col)[RED_CHANNEL] = cv::saturate_cast<uchar>(R);
        new_image->at<cv::Vec3b>(row, col)[GREEN_CHANNEL] = cv::saturate_cast<uchar>(G);
        new_image->at<cv::Vec3b>(row, col)[BLUE_CHANNEL] = cv::saturate_cast<uchar>(B);
    }
    else if (source_image->channels() == 1){
        new_image->at<uchar>(row, col) = cv::saturate_cast<uchar>(L);
    }
}

void ImageEditing::applyConvolution(cv::Mat* image, const std::vector<std::vector<double>> kernel){
    int cols = image->cols;
    int rows = image->rows;
    cv::Mat image_copy = image->clone();

    std::vector<std::vector<double>> kernel_copy;
    std::copy(kernel.begin(), kernel.end(), std::back_inserter(kernel_copy));
    
    rotateKernel(kernel_copy);

    #pragma omp parallel for collapse(2)
    for(int i = 1; i < rows - 1; i++){
        for(int j = 1; j < cols - 1; j++){
            applyConvolutionToPoint(image, &image_copy, i, j, kernel_copy);
        }
    }

    *image = image_copy;
    image_copy.release();
}

void ImageEditing::convertToNegative(cv::Mat* image){
    int cols = image->cols;
    int rows = image->rows;
    int channels = image->channels();

    #pragma omp parallel for collapse(3)
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            for(int c = 0; c < channels; c++){
                image->at<cv::Vec3b>(i, j)[c] = 255 - image->at<cv::Vec3b>(i, j)[c]; 
            }
        }
    }
}

void ImageEditing::changeContrast(cv::Mat* image, double modifier){
    if(modifier <= 0 || modifier > 255) return;
    
    int cols = image->cols;
    int rows = image->rows;
    int channels = image->channels();
    double new_value;

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            for(int c = 0; c < channels; c++){
                new_value = image->at<cv::Vec3b>(i, j)[c] * modifier;
                
                if(new_value > 255)
                    new_value = 255;
                else if(new_value < 0)
                    new_value = 0;
                
                image->at<cv::Vec3b>(i, j)[c] = (uchar) new_value;
            }
        }
    }
}


/*-------------------------------------
            QUANTIZATION
-------------------------------------*/

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
    #pragma omp parallel for collapse(2)
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

    #pragma omp parallel for collapse(2)
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){

            R = image->at<cv::Vec3b>(i, j)[RED_CHANNEL] * RED_COEFFICIENT;
            G = image->at<cv::Vec3b>(i, j)[GREEN_CHANNEL] * GREEN_COEFFICIENT;
            B = image->at<cv::Vec3b>(i, j)[BLUE_CHANNEL] * BLUE_COEFFICIENT;

            gray_scale_value = R + G + B;

            image->at<cv::Vec3b>(i, j)[RED_CHANNEL] = cv::saturate_cast<uchar>((uint) gray_scale_value);
            image->at<cv::Vec3b>(i, j)[GREEN_CHANNEL] = cv::saturate_cast<uchar>((uint) gray_scale_value);
            image->at<cv::Vec3b>(i, j)[BLUE_CHANNEL] = cv::saturate_cast<uchar>((uint) gray_scale_value);
        
        }
    }
}

/*---------------VERTICAL FLIP ALGORITHM-------------
    for i = 0 to i = image.rows
        swap(image.row(i), image.row(image.rows - i))
---------------------------------------------------*/

void swapImageRows(cv::Mat* image, uchar* top, uchar* bottom){
    int cols = image->cols * image->channels();

    uchar* temp = new uchar[cols];
    std::memcpy(temp, top, cols*sizeof(uchar));
    std::memcpy(top, bottom, cols*sizeof(uchar));
    std::memcpy(bottom, temp, cols*sizeof(uchar));

    delete[] temp;
}

void ImageEditing::mirrorVertical(cv::Mat* image){
    int rows = image->rows;

    uchar* top;
    uchar* bottom;

    #pragma omp parallel for 
    for(int i = 0; i < rows/2; i++){
        top = image->ptr<uchar>(i);
        bottom = image->ptr<uchar>(rows - i -1);    

        swapImageRows(image, top, bottom);
    }

}

/*---HORIZONTAL FLIP ALGORITHM-------------
    for i = 0 to i = image.cols
        swap(image.col(i), image.col(image.cols - i))
-----------------------------------------*/

void swapPixels(cv::Mat* image, uchar* left, uchar* right){
    int num_channels = image->channels();
    uchar* temp = new uchar[num_channels];
    
    std::memcpy(temp, left, num_channels);
    std::memcpy(left, right, num_channels);
    std::memcpy(right, temp, num_channels);
    
    delete[] temp;

}

void ImageEditing::mirrorHorizontal(cv::Mat* image){

    int num_channels = image->channels();
    int rows = image->rows;
    int cols = image->cols * num_channels;
    
    uchar* row;

    #pragma omp parallel for collapse(2)
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols/2; j+= num_channels){
            row = image->ptr<uchar>(i);

            swapPixels(image, &row[j], &row[cols - j - num_channels]);

        }
    }


}

/*---BRIGHNTES ADJUSTMENT ALGORITHM---------
    for every pixel in the image
        for every channel of the pixel
            pixel[channel] += adjustment
-----------------------------------------*/
void ImageEditing::changeBrightness(cv::Mat* image, int delta){
    
    bool is_gray_scale = image->channels() == 1;
    int new_value;

    #pragma omp parallel for collapse(2)
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
    #pragma omp parallel for collapse(2)
    for(int i = 0 ; i < rows; i++){
        for(int j = 0; j < cols; j++){
            tone = image.at<cv::Vec3b>(i, j)[0];
            if(tone > lightest_tone) lightest_tone = tone;
            if(tone < darkest_tone) darkest_tone = tone;
        }
    }

    return lightest_tone - darkest_tone + 1;
}


/*-------------------------------------
             HISTOGRAM
-------------------------------------*/

uchar findValueClosestTo(int shade, std::vector<double> target){
    int l = 0;
    int r = TONE_RANGE -1;
    int m;

    while(l <= r){
        m = (l + r) / 2;

        if(target[m] == shade){
            return target[m];
        }
        else if(shade < target[m]){
            r = m - 1;
        }
        else{
            l = m + 1;
        }
    }

    if(l == 0) return target[l];
    if(r == TONE_RANGE - 1) return target[r];

    return fabs(target[m] - shade) < fabs(target[m + 1] - shade) ? 
        target[m] : target[m + 1];
}

Histogram ImageEditing::generateHistogram(const cv::Mat* image){
    Histogram hist = Histogram(image);
    return hist;
}

Histogram ImageEditing::generateCumulativeHistogram(const cv::Mat* image){
    Histogram hist = Histogram(image, true);
    return hist;
}

void ImageEditing::equalizeHistogram(cv::Mat* image, bool image_is_grayscale){
    Histogram hist_cum = Histogram(image, true);
    std::vector<double> L = hist_cum.getLuminanceChannel();
    std::vector<double> R = hist_cum.getRedChannel();
    std::vector<double> G = hist_cum.getGreenChannel();
    std::vector<double> B = hist_cum.getBlueChannel();

    int cols = image->cols;
    int rows = image->rows;


    if(image_is_grayscale || image->channels() == 1){
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                image->at<cv::Vec3b>(i, j)[RED_CHANNEL] = R[image->at<cv::Vec3b>(i, j)[RED_CHANNEL]];
                image->at<cv::Vec3b>(i, j)[GREEN_CHANNEL] = G[image->at<cv::Vec3b>(i, j)[GREEN_CHANNEL]];
                image->at<cv::Vec3b>(i, j)[BLUE_CHANNEL] = B[image->at<cv::Vec3b>(i, j)[BLUE_CHANNEL]];
            }
        }
    }
    else{
        for(int i = 0; i < rows; i++){
            for(int j = 0; j < cols; j++){
                
                image->at<cv::Vec3b>(i, j)[RED_CHANNEL] = R[image->at<cv::Vec3b>(i, j)[RED_CHANNEL]];
                image->at<cv::Vec3b>(i, j)[GREEN_CHANNEL] = G[image->at<cv::Vec3b>(i, j)[GREEN_CHANNEL]];
                image->at<cv::Vec3b>(i, j)[BLUE_CHANNEL] = B[image->at<cv::Vec3b>(i, j)[BLUE_CHANNEL]];
            }
        }
    }
}

void ImageEditing::matchHistogram(cv::Mat* image, const cv::Mat* reference){

    std::vector<uchar> mapping(256, 0);
    Histogram hist_ref = Histogram(reference, true);
    Histogram hist_dest = Histogram(image, true);
    uchar closest;

    for(int i = 0; i < TONE_RANGE; i++){
        closest = cv::saturate_cast<uchar>(findValueClosestTo(i, hist_ref.getLuminanceChannel()));
        mapping[i] = closest;
    }

    int cols = image->cols;
    int rows = image->rows;

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){

            if(image->channels() == 3){
                image->at<cv::Vec3b>(i, j)[RED_CHANNEL] = mapping[image->at<cv::Vec3b>(i, j)[RED_CHANNEL]];
                image->at<cv::Vec3b>(i, j)[GREEN_CHANNEL] = mapping[image->at<cv::Vec3b>(i, j)[GREEN_CHANNEL]];
                image->at<cv::Vec3b>(i, j)[BLUE_CHANNEL] = mapping[image->at<cv::Vec3b>(i, j)[BLUE_CHANNEL]];
            }
            else{
                image->at<uchar>(i, j) = mapping[image->at<uchar>(i, j)];
            }
        }
    }
}