#include "foregroundremove.hpp"

#include <cstdlib>
#include <cmath>

/**
 * @brief Return the index of the inlier pixels
 * @return std::vector<int> the index of the inliers
 */
std::vector<int> ForegroundRemove::getInliers(Image::PixelPtr pixelBuffer,
    int pixelSize, int bufSize, int selectedindex, float threshold) {
    Image::PixelPtr pixelSelected = &pixelBuffer[selectedindex*pixelSize];
    std::vector<int> inliers;
    for (int i = 0; i < bufSize; i+=pixelSize) {
        // calc the Frobenius norm
        float norm = 0.0;
        for (int j = 0; j < pixelSize; j++) {
            int x = pixelBuffer[i+j]-pixelSelected[j];
            norm += x*x;
        }
        if (sqrt(norm) < threshold) {
            inliers.push_back(i/pixelSize);
        }
    }

    return inliers;
}

std::vector<int> ForegroundRemove::getBestInliers(const int col, const int row) {
    int channels = sourceImages[0]->getChannels();
    int pixelSize = sizeof(Image::PixelType)*channels;
    int bufSize = pixelSize*sourceImages.size();
    std::unique_ptr<Image::PixelType[]> buf(new Image::PixelType[bufSize]);

    // copy vector of row pixels from volume to buffer
    for (int i = 0; i < sourceImages.size(); i++) {
        Image::PixelPtr p = sourceImages[i]->getPixelAt(col, row);
        std::memcpy(&buf[pixelSize*i], p, pixelSize);
    }

    int selectedindex = rand()%sourceImages.size();
    auto inliers = getInliers(&buf[0], pixelSize, bufSize, selectedindex, 0.5);

    return inliers;
}

std::unique_ptr<Image::PixelType[]> ForegroundRemove::calcFittestBackgroudPixel(
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
    int pixelSize = sizeof(Image::PixelType)*channels;
    std::unique_ptr<float[]> tmp(new float[pixelSize]{});
    for (auto i: bestInliers) {
        Image::PixelPtr img_pixel = sourceImages[i]->getPixelAt(col, row);
        for (int j = 0; j < channels; j++) {
            tmp[j] += img_pixel[j];
        }
    }

    // calculate the mean pixel value
    std::unique_ptr<Image::PixelType[]> mean_pixel(new Image::PixelType[channels]);
    for (int i = 0; i < channels; i++) {
        mean_pixel[i] = static_cast<Image::PixelType>(tmp[i]/bestInliers.size());
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
            resultImage->setPixelAt(col, row, pixel.get());
        }
    }
}

Image::ImagePtr ForegroundRemove::getResult() {
    return std::move(resultImage);
}