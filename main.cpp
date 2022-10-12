#include <opencv2/opencv.hpp>
#include <experimental/filesystem>
#include <iostream>
#include <memory>
#include <vector>

#include <chrono>
#include <thread>

#include "rgbimage.hpp"
#include  "foregroundremove.hpp"
#include  "foregroundremoveparallel.hpp"

namespace fs = std::experimental::filesystem;

std::vector<Image::ImagePtr> images;

void loadImages(const std::string &path) {
	for (const auto & entry : fs::directory_iterator(path)) {
		if (entry.path().extension().compare(".jpg") == 0 ||
		    entry.path().extension().compare(".png") == 0) {
			cv::Mat image = cv::imread(entry.path(), 1);
			if (image.data == nullptr) {
				std::string err{"Can't read image: "};
				throw std::runtime_error(err.append(entry.path()));
			}
			printf("(h=%d,w=%d) - %s\n",
				image.rows, image.cols, entry.path().c_str());
			Image::ImagePtr rgbImg = std::make_unique<RGBImage>(image);
			if (images.size() > 0) {
				if (images[0]->getCols() != image.cols && images[0]->getRows() != image.rows) {
					std::stringstream err;
					err << "Invalid image size (" <<
						image.rows << "," <<
						image.cols << ") for " << entry.path();
					throw std::runtime_error(err.str());
				}
			}
			images.push_back(std::move(rgbImg));
			// if (images.size() == 100) return;
		}
	}
}

void showImage(cv::Mat &image) {
	std::string windowName = "Result Image";
 	cv::namedWindow(windowName);
 	cv::imshow(windowName, image);
 	cv::waitKey(0);
 	cv::destroyWindow(windowName);
}

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("usage: %s <images_path>\n", argv[0]);
		return -1;
	}

	loadImages(argv[1]);

    int cols = images[0]->getCols();
    int rows = images[0]->getRows();
	Image::ImagePtr buf = std::make_unique<RGBImage>(cols, rows);
	Algorithm<Image::ImagePtr> *algo = new ForegroundRemoveParallel{images, std::move(buf)};
	algo->execute();
	auto sptr = algo->getResult();
	RGBImage* resultImage = static_cast<RGBImage*>(&(*sptr));
	cv::imwrite("./foreground_remove_result.jpg", resultImage->getCv2());
	return 0;
}
