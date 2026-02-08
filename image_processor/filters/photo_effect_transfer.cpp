#include "photo_effect_transfer.h"
#include <cmath>
#include <algorithm>

static uint8_t Clamp(int value) {
    return static_cast<uint8_t>(std::clamp(value, image::utils::MIN_COLOR, image::utils::MAX_COLOR));
}

uint8_t Desaturate(uint8_t r, uint8_t g, uint8_t b) {
    return static_cast<uint8_t>(image::utils::RED_WEIGHT * r + image::utils::GREEN_WEIGHT * g +
                                image::utils::BLUE_WEIGHT * b);
}

Image filter::PhotoEffectTransfer::Apply(Image& image) {
    size_t width = image.GetWidth();
    size_t height = image.GetHeight();
    std::vector<std::vector<Color>> result(height, std::vector<Color>(width));

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            Color orig = image.GetColor(x, y);

            uint8_t base = Desaturate(orig.red, orig.green, orig.blue);

            int red = base + image::utils::RED_SHIFT;
            int green = base + image::utils::GREEN_SHIFT;
            int blue = base + image::utils::BLUE_SHIFT;

            if (base > image::utils::HIGHLIGHT_THRESHOLD) {
                red += image::utils::RED_HIGHLIGHT_BONUS;
                green += image::utils::GREEN_HIGHLIGHT_BONUS;
                blue += image::utils::BLUE_HIGHLIGHT_BONUS;
            }

            result[y][x] = Color(Clamp(blue), Clamp(green), Clamp(red));
        }
    }

    return Image(result);
}
