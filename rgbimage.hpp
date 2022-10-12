#pragma once

#include <opencv2/opencv.hpp>
#include "image.hpp"

class RGBImage: public Image {
    static const int RGB_CHANNELS = 3;
    cv::Mat img;
public:
    RGBImage() = delete;
    RGBImage(const cv::Mat &img);
    RGBImage(const int &cols, const int &rows);
    RGBImage(const RGBImage&) = delete;
    cv::Mat& getCv2() { return img; }
    RGBImage& operator=(const RGBImage&) = delete;
    PixelType getPixelAt(const int x, const int y) override;
    void setPixelAt(const int x, const int y, const PixelType& pixel) override;
    ~RGBImage() = default;
};