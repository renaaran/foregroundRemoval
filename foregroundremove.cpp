#include "foregroundremove.hpp"

#include <cstdlib>
#include <cmath>

/**
 * @brief Return the index of the inlier pixels
 * @return std::vector<int> the index of the inliers
 */
std::vector<int> ForegroundRemove::getInliers(Image::PixelPtr pixelBuffer,
    int pixelSize, int bufSize, int selectedindex, float threshold) {
    Image::PixelPtr pixelSelected = &pixelBuffer[selectedindex];
    std::vector<int> inliers;
    for (int i = 0; i < bufSize; i+=pixelSize) {
        // calc the Frobenius norm
        float norm = 0.0;
        for (int j = 0; j < pixelSize; j++) {
            int x = pixelBuffer[i+j]-pixelSelected[j];
            norm += x*x;
        }
        if (sqrt(norm) < threshold) {
            inliers.push_back(i);
        }
    }
    return inliers;
}

std::vector<int> ForegroundRemove::getBestInliers(const int col, const int row) {
    int channels = images[0]->getChannels();
    int pixelSize = sizeof(Image::PixelType)*channels;
    int bufSize = pixelSize*images.size();
    std::unique_ptr<Image::PixelType> buf(new Image::PixelType[bufSize]);
    Image::PixelPtr pb = buf.get();

    // copy vector of row pixels from volume to buffer
    for (int i = 0; i < images.size(); i++) {
        Image::PixelPtr p = images[i]->getPixelAt(col, row);
        std::memcpy(&pb[pixelSize*i], p, sizeof(pixelSize));
    }

    int selectedindex = rand()%images.size();
    auto inliers = getInliers(pb, pixelSize, bufSize, selectedindex, 0.5);
    return inliers;
}

Image::PixelPtr ForegroundRemove::calcFittestBackgroudPixel(const int col,
    const int row, int minInliers) {
    const int MAX_ATTEMPT = 10;
    std::vector<int> bestInliers;
    int n = 0;
    while (bestInliers.size() < minInliers && n < MAX_ATTEMPT) {
        auto inliers = getBestInliers(col ,row);
        if (inliers.size() > bestInliers.size() && inliers.size() > 0) {
            // printf("(%d) bestInliers=%ld\n", n, inliers.size());
            bestInliers = inliers;
        }
        ++n;
    }
    return nullptr;
}

void ForegroundRemove::execute() {
    int rows = images[0]->getRows();
    int cols = images[0]->getCols();
    for (int i = 0; i < rows; i++) {
        printf("%d\n", i);
        for (int j = 0; j < cols; j++) {
            Image::PixelPtr p = calcFittestBackgroudPixel(j, i);
        }
    }
}

Image::ImagePtr ForegroundRemove::getResult() {
    return nullptr;
}