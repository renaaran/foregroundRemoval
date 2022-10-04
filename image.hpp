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
    Image(const int &cols, const int &rows):
        img{cv::Mat::zeros(rows, cols, CV_8UC3)} {}
    Image(const cv::Mat &img):
        img{img.clone()}, channels{img.channels()} {}
    int getCols() { return img.cols; }
    int getRows() { return img.rows; }
    int getChannels() { return channels; }
    cv::Mat& getCv2() { return img; }
    virtual PixelPtr getPixelAt(const int x, const int y) = 0;
    virtual void setPixelAt(const int x, const int y, const PixelPtr pixel) = 0;
    virtual ~Image() = default;
};