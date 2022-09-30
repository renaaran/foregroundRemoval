#include "rgbimage.hpp"

RGBImage::RGBImage(const cv::Mat &img) : Image(img) {
    if (img.channels() != 3) {
        throw std::runtime_error("RGB image must have 3 channels!");
    }
    // std::cout << "[RGBImage] constructor\n";
}

Image::PixelPtr RGBImage::getPixelAt(const int x, const int y) {
    static u_int8_t pixel[3];
    pixel[0] = img.data[y*img.cols*channels + x*channels + 2]; // R
    pixel[1] = img.data[y*img.cols*channels + x*channels + 1]; // G
    pixel[2] = img.data[y*img.cols*channels + x*channels + 0]; // B
    return pixel;
}