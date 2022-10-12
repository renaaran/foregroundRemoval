#include "foregroundremoveparallel.hpp"
#include "threadpool.hpp"

#include <cstdlib>
#include <cmath>

void ForegroundRemoveParallel::execute() {
    unsigned int cores = std::thread::hardware_concurrency();
    ThreadPool pool(cores);

    int rows = sourceImages[0]->getRows();
    int cols = sourceImages[0]->getCols();
    for (int row = 0; row < rows; row++) {
        std::vector<std::future<int>> results;
        printf("row=%d\n", row);
        for (int col = 0; col < cols; col++) {
            results.emplace_back(pool.enqueue([this, row, col] {
                auto pixel = calcFittestBackgroudPixel(col, row);
                resultImage->setPixelAt(col, row, pixel);
                return 0;
            }));
        }
        for(auto& result: results) result.get();
    }
}
