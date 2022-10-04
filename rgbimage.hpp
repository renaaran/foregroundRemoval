#pragma once

#include "image.hpp"

class RGBImage: public Image {
    static const int RGB_CHANNELS = 3;
public:
    RGBImage(const cv::Mat &img);
    RGBImage(const int &cols, const int &rows);
    RGBImage(const RGBImage&) = delete;
    RGBImage& operator=(const RGBImage&) = delete;
    PixelPtr getPixelAt(const int x, const int y) override;
    void setPixelAt(const int x, const int y, const PixelPtr pixel) override;
    ~RGBImage() = default;
};