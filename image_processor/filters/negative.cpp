#include "negative.h"

Image filter::Negative::Apply(Image& image) {
    size_t width = image.GetWidth();
    size_t height = image.GetHeight();

    std::vector<std::vector<Color>> result(height, std::vector<Color>(width));

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            const Color& original = image.GetColor(x, y);
            Color inverted;
            inverted.red = image::utils::MAX_COLOR_VALUE - original.red;
            inverted.green = image::utils::MAX_COLOR_VALUE - original.green;
            inverted.blue = image::utils::MAX_COLOR_VALUE - original.blue;
            result[y][x] = inverted;
        }
    }

    return Image(result);
}
