#pragma once

#include "image.hpp"

class RGBImage: public Image {
public:
    RGBImage(const cv::Mat &img);
    RGBImage(const RGBImage&) = delete;
    RGBImage& operator=(const RGBImage&) = delete;
    PixelPtr getPixelAt(const int x, const int y) override;
    ~RGBImage() {
        // std::cout << "[RGBImage] destructor\n";
    }
};