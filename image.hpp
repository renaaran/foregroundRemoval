#pragma once

#include <opencv2/opencv.hpp>
#include <variant>

class Image {
protected:
    cv::Mat img;
    int channels;
    Image();
public:
    using PixelType = uint8_t;
    using PixelPtr = PixelType*;
    using ImagePtr = std::unique_ptr<Image>;
    Image(const cv::Mat &img):
        img{img}, channels{img.channels()} {
            // std::cout << "[Image] constructor\n";
        }
    int getCols() { return img.cols; }
    int getRows() { return img.rows; }
    int getChannels() { return channels; }
    virtual PixelPtr getPixelAt(const int x, const int y) = 0;
    virtual ~Image() {
        // std::cout << "[Image] destructor\n";
    };
};