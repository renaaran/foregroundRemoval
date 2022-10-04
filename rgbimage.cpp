#include "rgbimage.hpp"

RGBImage::RGBImage(const cv::Mat &img) : Image(img) {
    if (img.channels() != RGB_CHANNELS) {
        throw std::runtime_error("RGB image must have 3 channels!");
    }
}

RGBImage::RGBImage(const int &cols, const int &rows): Image(cols, rows) {
    printf("[RGBImage]: cols=%d, rows=%d\n", cols, rows);
}

Image::PixelPtr RGBImage::getPixelAt(const int x, const int y) {
    static Image::PixelType pixel[RGB_CHANNELS];
    cv::Vec3b color = img.at<cv::Vec3b>(y, x);
    pixel[0] = color[2];
    pixel[1] = color[1];
    pixel[2] = color[0];
    return pixel;
}

void RGBImage::setPixelAt(const int x, const int y, const PixelPtr pixel) {
    cv::Vec3b color = img.at<cv::Vec3b>(y, x);
    color[2] = pixel[0];
    color[1] = pixel[1];
    color[0] = pixel[2];
    img.at<cv::Vec3b>(y, x) = color;
}