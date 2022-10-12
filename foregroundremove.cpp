#include "foregroundremove.hpp"

#include <cstdlib>
#include <vector>
#include <cmath>

/**
 * @brief Return the index of the inlier pixels
 * @return std::vector<int> the index of the inliers
 */
std::vector<int> ForegroundRemove::getInliers(std::vector<Image::PixelType>& pixelBuffer,
    int selectedindex, float threshold) {
    Image::PixelType pixelSelected = pixelBuffer[selectedindex];
    std::vector<int> inliers;
    for (uint i = 0; i < pixelBuffer.size(); i++) {
        // calc the Frobenius norm
        float norm = 0.0;
        for (uint j = 0; j < pixelSelected.size(); j++) {
            int x = pixelBuffer[i][j]-pixelSelected[j];
            norm += x*x;
        }
        if (sqrt(norm) < threshold) {
            inliers.push_back(i);
        }
    }

    return inliers;
}

std::vector<int> ForegroundRemove::getBestInliers(const int col, const int row) {
    //int channels = sourceImages[0]->getChannels();
    std::vector<Image::PixelType> buf;

    // copy vector of row pixels from volume to buffer
    for (int i = 0; i < sourceImages.size(); i++) {
        buf.emplace_back(sourceImages[i]->getPixelAt(col, row));
    }

    int selectedindex = rand()%sourceImages.size();
    auto inliers = getInliers(buf, selectedindex);

    return inliers;
}

Image::PixelType ForegroundRemove::calcFittestBackgroudPixel(
    const int col, const int row, int minInliers) {
    const int MAX_ATTEMPT = 10;
    std::vector<int> bestInliers;
    int n = 0;
    while (bestInliers.size() < minInliers && n < MAX_ATTEMPT) {
        auto inliers = getBestInliers(col, row);
        if (inliers.size() > bestInliers.size() && inliers.size() > 0) {
            bestInliers = inliers;
        }
        ++n;
    }

    // sum the pixels value per channel
    int channels = sourceImages[0]->getChannels();
    std::vector<float> tmp(channels);
    for (auto i: bestInliers) {
        Image::PixelType img_pixel = sourceImages[i]->getPixelAt(col, row);
        for (int j = 0; j < channels; j++) {
            tmp[j] += img_pixel[j];
        }
    }

    // calculate the mean pixel value
    Image::PixelType mean_pixel(channels);
    for (int i = 0; i < channels; i++) {
        mean_pixel[i] = tmp[i]/bestInliers.size();
    }

    return mean_pixel;
}

void ForegroundRemove::execute() {
    int rows = sourceImages[0]->getRows();
    int cols = sourceImages[0]->getCols();
    for (int row = 0; row < rows; row++) {
        printf("row=%d\n", row);
        for (int col = 0; col < cols; col++) {
            auto pixel = calcFittestBackgroudPixel(col, row);
            resultImage->setPixelAt(col, row, pixel);
        }
    }
}

Image::ImagePtr ForegroundRemove::getResult() {
    return std::move(resultImage);
}