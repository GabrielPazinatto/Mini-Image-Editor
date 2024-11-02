#include <vector>
#include <opencv2/opencv.hpp>

#define TONE_RANGE 256

class Histogram{
    private:
        std::vector<double> red_channel;
        std::vector<double> green_channel;
        std::vector<double> blue_channel;
        std::vector<double> luminance_channel;

    public:

        Histogram(const cv::Mat* image, bool cumulative = false);

        std::vector<double>& getRedChannel();
        std::vector<double>& getGreenChannel();
        std::vector<double>& getBlueChannel();
        std::vector<double>& getLuminanceChannel();
};