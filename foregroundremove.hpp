#pragma once

#include "image.hpp"
#include "algorithm.hpp"

class ForegroundRemove: public Algorithm<Image::ImagePtr> {
    const std::vector<Image::ImagePtr> &images;

    Image::PixelPtr calcFittestBackgroudPixel(const int row,
        const int col, int minInliers = 50);
    std::vector<int> getInliers(Image::PixelPtr pixelBuffer,
        int pixelSize, int bufSize, int selectedindex, float threshold);
    std::vector<int> getBestInliers(const int col, const int row);
public:
    ForegroundRemove(const std::vector<Image::ImagePtr> &images) : images{images} {}
    void execute();
    Image::ImagePtr getResult();
};