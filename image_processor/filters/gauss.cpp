#include "gauss.h"
#include <cmath>
#include <algorithm>

namespace {

inline uint8_t Clamp255(double value) {
    return static_cast<uint8_t>(std::clamp(static_cast<int32_t>(std::lround(value)), image::utils::ZERO,
                                           static_cast<int32_t>(image::utils::MAXL_COLOR_VALUE)));
}

}  // namespace

filter::GaussianBlurFilter::GaussianBlurFilter(float sigma) {
    sigma_ = sigma;
}

std::vector<int> filter::GaussianBlurFilter::BoxesForGauss(float sigma, double number) {
    double width_ideal = std::sqrt(image::utils::NORMING * sigma * sigma / number + 1);
    int width_lower = static_cast<int>(std::floor(width_ideal));
    if (width_lower % 2 == 0) {
        --width_lower;
    }
    int width_upper = width_lower + 2;
    double m_ideal = (image::utils::NORMING * sigma * sigma - number * width_lower * width_lower -
                      4 * number * width_lower - 3 * number) /
                     (-4 * width_lower - 4);
    double m = std::round(m_ideal);
    std::vector<int> sizes;
    for (int i = 0; i < static_cast<int>(number); ++i) {
        sizes.push_back(i < m ? width_lower : width_upper);
    }
    return sizes;
}

void filter::GaussianBlurFilter::BoxBlur(const std::vector<Color>& source, std::vector<Color>& target, size_t width,
                                         size_t height, int radius) {
    for (size_t i = 0; i < source.size(); ++i) {
        target[i] = source[i];
    }
    BoxBlurH(source, target, width, height, radius);
    BoxBlurT(source, target, width, height, radius);
}

void filter::GaussianBlurFilter::BoxBlurH(const std::vector<Color>& source, std::vector<Color>& target, size_t width,
                                          size_t height, int radius) {
    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            double new_red = 0;
            double new_green = 0;
            double new_blue = 0;
            int count = 0;

            int j_int = static_cast<int>(j);
            for (int x = j_int - radius; x <= j_int + radius; ++x) {
                int clamped_x = std::clamp(x, 0, static_cast<int>(width) - 1);
                const Color& c = source[i * width + static_cast<size_t>(clamped_x)];

                new_red += c.red;
                new_green += c.green;
                new_blue += c.blue;
                ++count;
            }

            target[i * width + j] =
                Color(Clamp255(new_blue / count), Clamp255(new_green / count), Clamp255(new_red / count));
        }
    }
}

void filter::GaussianBlurFilter::BoxBlurT(const std::vector<Color>& source, std::vector<Color>& target, size_t width,
                                          size_t height, int radius) {
    for (size_t i = 0; i < width; ++i) {
        for (size_t j = 0; j < height; ++j) {
            double new_red = 0;
            double new_green = 0;
            double new_blue = 0;
            int count = 0;

            int j_int = static_cast<int>(j);
            for (int y = j_int - radius; y <= j_int + radius; ++y) {
                int clamped_y = std::clamp(y, 0, static_cast<int>(height) - 1);
                const Color& c = source[static_cast<size_t>(clamped_y) * width + i];

                new_red += c.red;
                new_green += c.green;
                new_blue += c.blue;
                ++count;
            }

            target[j * width + i] =
                Color(Clamp255(new_blue / count), Clamp255(new_green / count), Clamp255(new_red / count));
        }
    }
}

Image filter::GaussianBlurFilter::Apply(Image& image) {
    size_t width = image.GetWidth();
    size_t height = image.GetHeight();
    size_t size = width * height;

    std::vector<Color> flat_source;
    flat_source.reserve(size);
    for (const auto& row : image.GetData()) {
        for (const auto& c : row) {
            flat_source.push_back(c);
        }
    }

    std::vector<Color> temp(size);
    std::vector<Color> result(size);

    std::vector<int> boxes = BoxesForGauss(sigma_, 3);
    int r1 = (boxes[0] - 1) / 2;
    int r2 = (boxes[1] - 1) / 2;
    int r3 = (boxes[2] - 1) / 2;

    BoxBlurH(flat_source, temp, width, height, r1);
    BoxBlurT(temp, result, width, height, r1);

    BoxBlurH(result, temp, width, height, r2);
    BoxBlurT(temp, result, width, height, r2);

    BoxBlurH(result, temp, width, height, r3);
    BoxBlurT(temp, result, width, height, r3);

    std::vector<std::vector<Color>> new_data(height, std::vector<Color>(width));
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            new_data[y][x] = result[y * width + x];
        }
    }

    return Image(new_data);
}
