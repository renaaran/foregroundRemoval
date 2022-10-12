#pragma once

#include <cstdint>
#include <memory>
#include <vector>

class Image {
protected:
    int cols, rows;
    int channels;
public:
    using PixelType = std::vector<uint8_t>;
    using ImagePtr = std::unique_ptr<Image>;
    Image() = delete;
    Image(const int &cols, const int &rows, const int &channels):
        cols{cols}, rows{rows}, channels{channels} {};
    int getCols() const { return cols; }
    int getRows() const { return rows; }
    int getChannels() const { return channels; }
    virtual PixelType getPixelAt(const int x, const int y) = 0;
    virtual void setPixelAt(const int x, const int y, const PixelType& pixel) = 0;
    virtual ~Image() = default;
};