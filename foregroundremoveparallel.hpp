#pragma once

#include "foregroundremove.hpp"

class ForegroundRemoveParallel: public ForegroundRemove {
public:
    ForegroundRemoveParallel(const std::vector<Image::ImagePtr> &sourceImages,
        Image::ImagePtr resultImage): ForegroundRemove(sourceImages, std::move(resultImage)) {}
    void execute() override;
};