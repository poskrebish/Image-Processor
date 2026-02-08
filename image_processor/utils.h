#include <array>
#include <cstddef>
#include <cstdint>
#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

namespace image::utils {
const uint32_t BMP_HEADER_SIZE = 14;
const uint32_t DIB_HEADER_SIZE = 40;
const uint32_t F_B_FIRST_POS = 0;
const uint32_t F_B_SECOND_POS = 1;
const uint32_t WIDTH_POSITION = 4;
const uint32_t HEIGHT_POSITION = 8;
const uint32_t BYTES_PER_PIXEL = 3;
const uint32_t INPUT_FILE_POSITION = 0;
const uint32_t OUTPUT_FILE_POSITION = 1;
const size_t CNT_OF_PLANES = 1;
const size_t CNT_OF_PLANES_POSITION = 12;
const size_t DATA_SIZE_POSITION = 20;
const std::array<double, 3> GS_CF = {0.299, 0.587, 0.114};
const std::array<char, 2> FORMAT_BYTES = {'B', 'M'};
const std::array<int32_t, 3> SHIFT_BYTES = {8, 16, 24};
const double NORMING = 12.0;
const uint8_t MAX_COLOR_VALUE = 255;
const double GRAYSCALE_DIVISOR = 255.0;
const uint8_t BIT_DEPTH = 8;
const uint8_t DIB_HEADER_SIZE_BYTE = 40;
const uint8_t OFFSET_BIT_DEPTH = 14;
const uint8_t OFFSET_DIB_DEPTH = 24;
const uint8_t BMP_HEADER_OFFSET = 10;
const uint8_t BYTE_MASK = 0xFF;
const int MIN_COLOR = 0;
const int MAX_COLOR = 255;
const double RED_WEIGHT = 0.3;
const double GREEN_WEIGHT = 0.59;
const double BLUE_WEIGHT = 0.11;
const int RED_SHIFT = 25;
const int GREEN_SHIFT = 5;
const int BLUE_SHIFT = -10;
const uint8_t HIGHLIGHT_THRESHOLD = 180;
const int RED_HIGHLIGHT_BONUS = 15;
const int GREEN_HIGHLIGHT_BONUS = 10;
const int BLUE_HIGHLIGHT_BONUS = 5;
const double BLUR = 2.0;
const auto MAXL_COLOR_VALUE = 255L;
const int32_t ZERO = 0;
const float TWO = 2.0f;
const int TESTR_TEN = 10;
const int TESTR_FIVE = 5;
const int TESTR_FIVETEEN = 15;
const int TESTR_ONE = 1;

};  // namespace image::utils
#endif