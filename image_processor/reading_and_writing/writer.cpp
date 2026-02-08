#include "writer.h"
#include "../utils.h"
#include <fstream>

namespace read_and_write {

size_t GetPaddingSize(size_t width) {
    return (4 - (width * image::utils::BYTES_PER_PIXEL) % 4) % 4;
}

Writer::Writer(std::string filename) : path_(std::move(filename)) {
}

void Writer::NumberToBytes(size_t number, unsigned char* bytes) {
    for (size_t i = 0; i <= image::utils::SHIFT_BYTES.size(); ++i) {
        bytes[i] = static_cast<unsigned char>((number >> (i == 0 ? 0 : image::utils::SHIFT_BYTES[i - 1])) &
                                              image::utils::BYTE_MASK);
    }
}

void Writer::WriteDIBHeader(unsigned char* dib_header, size_t width, size_t height) {
    for (size_t i = 0; i < image::utils::DIB_HEADER_SIZE; ++i) {
        dib_header[i] = 0;
    }

    NumberToBytes(image::utils::DIB_HEADER_SIZE, dib_header);
    NumberToBytes(width, dib_header + image::utils::WIDTH_POSITION);
    NumberToBytes(height, dib_header + image::utils::HEIGHT_POSITION);
    NumberToBytes(1, dib_header + image::utils::CNT_OF_PLANES_POSITION);
    NumberToBytes(image::utils::OFFSET_DIB_DEPTH, dib_header + image::utils::OFFSET_BIT_DEPTH);

    size_t padding = GetPaddingSize(width);
    size_t raw_size = height * (width * image::utils::BYTES_PER_PIXEL + padding);
    NumberToBytes(raw_size, dib_header + image::utils::DATA_SIZE_POSITION);
}

void Writer::WriteBMPHeader(unsigned char* bmp_header, size_t file_size) {
    bmp_header[0] = 'B';
    bmp_header[1] = 'M';
    NumberToBytes(file_size, bmp_header + 2);
    bmp_header[image::utils::BMP_HEADER_OFFSET] =
        static_cast<unsigned char>(image::utils::BMP_HEADER_SIZE + image::utils::DIB_HEADER_SIZE);
}

void Writer::WriteImage(const Image& image) const {
    const size_t width = image.GetWidth();
    const size_t height = image.GetHeight();
    const size_t padding = GetPaddingSize(width);
    const size_t row_size = width * image::utils::BYTES_PER_PIXEL + padding;
    const size_t file_size = image::utils::BMP_HEADER_SIZE + image::utils::DIB_HEADER_SIZE + height * row_size;

    std::ofstream file(path_, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path_);
    }

    unsigned char bmp_header[image::utils::BMP_HEADER_SIZE] = {};
    unsigned char dib_header[image::utils::DIB_HEADER_SIZE] = {};

    WriteBMPHeader(bmp_header, file_size);
    WriteDIBHeader(dib_header, width, height);

    file.write(reinterpret_cast<char*>(bmp_header), image::utils::BMP_HEADER_SIZE);
    file.write(reinterpret_cast<char*>(dib_header), image::utils::DIB_HEADER_SIZE);

    unsigned char pad[3] = {0, 0, 0};

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            const Color& color = image.GetColor(x, y);
            unsigned char pixel[3] = {color.blue, color.green, color.red};
            file.write(reinterpret_cast<char*>(pixel), image::utils::BYTES_PER_PIXEL);
        }
        file.write(reinterpret_cast<char*>(pad), static_cast<std::streamsize>(padding));
    }

    file.close();
}

}  // namespace read_and_write
