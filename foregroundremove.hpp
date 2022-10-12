#pragma once

#include <vector>

#include "image.hpp"
#include "algorithm.hpp"

class ForegroundRemove: public Algorithm<Image::ImagePtr> {
protected:
    const std::vector<Image::ImagePtr> &sourceImages;
    Image::ImagePtr resultImage;

    Image::PixelType calcFittestBackgroudPixel(const int row,
        const int col, int minInliers = 50);
    std::vector<int> getInliers(std::vector<Image::PixelType>& pixelBuffer,
        int selectedindex, float threshold = 0.5);
    std::vector<int> getBestInliers(const int col, const int row);
public:
    ForegroundRemove(const std::vector<Image::ImagePtr> &sourceImages,
        Image::ImagePtr resultImage):
            sourceImages{sourceImages}, resultImage{std::move(resultImage)} {}
    void execute();
    Image::ImagePtr getResult();
};