#include <opencv2/opencv.hpp>
#include <experimental/filesystem>
#include <iostream>
#include <memory>
#include <vector>

#include <chrono>
#include <thread>

#include "rgbimage.hpp"
#include  "foregroundremove.hpp"

namespace fs = std::experimental::filesystem;

std::vector<Image::ImagePtr> images;

void load_images(const std::string &path) {
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
			Image::ImagePtr img = std::make_unique<RGBImage>(image);
			if (images.size() > 0) {
				if (images[0]->getCols() != image.cols && images[0]->getRows() != image.rows) {
					std::stringstream err;
					err << "Invalid image size (" <<
						image.rows << "," <<
						image.cols << ") for " << entry.path();
					throw std::runtime_error(err.str());
				}
			}
			images.push_back(std::move(img));
			// if (images.size() == 50) return;
		}
	}
}

int main(int argc, char** argv)
{
	if (argc != 2) {
		printf("usage: %s <images_path>\n", argv[0]);
		return -1;
	}

	load_images(argv[1]);
	// std::this_thread::sleep_for(std::chrono::seconds(60));

	Algorithm<Image::ImagePtr> *algo = new ForegroundRemove{images};
	algo->execute();
	Image::ImagePtr res = algo->getResult();
	return 0;
}
