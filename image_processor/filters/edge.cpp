#include "edge.h"
#include "grayscale.h"
#include <algorithm>

namespace {
const std::vector<std::vector<int>> EDGE_KEREL = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
}

filter::EdgeDetection::EdgeDetection(double threshold) : threshold_(threshold) {
}

Image filter::EdgeDetection::Apply(Image& image) {
    filter::Grayscale to_gray;
    Image gray = to_gray.Apply(image);

    size_t height = gray.GetHeight();
    size_t width = gray.GetWidth();

    std::vector<std::vector<Color>> output(height, std::vector<Color>(width));

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            int sum = 0;
            for (int ky = -1; ky <= 1; ++ky) {
                for (int kx = -1; kx <= 1; ++kx) {
                    int ny = std::clamp<int>(static_cast<int>(y) + ky, 0, static_cast<int>(height) - 1);
                    int nx = std::clamp<int>(static_cast<int>(x) + kx, 0, static_cast<int>(width) - 1);
                    int weight = EDGE_KEREL[ky + 1][kx + 1];
                    sum += weight * gray.GetColor(nx, ny).red;
                }
            }

            uint8_t result = (sum > static_cast<int>(threshold_ * image::utils::MAX_COLOR_VALUE))
                                 ? image::utils::MAX_COLOR_VALUE
                                 : 0;
            output[y][x] = Color(result, result, result);
        }
    }

    return Image(output);
}
