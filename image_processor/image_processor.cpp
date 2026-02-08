#include "utils.h"
#include <iostream>
#include "parser/parser.h"
#include "image/image.h"
#include "reading_and_writing/reader.h"
#include "filters/filters.h"
#include "reading_and_writing/writer.h"

std::vector<parser::Argument> GetArguments(int argc, char **argv) {
    std::vector<parser::Argument> arguments = parser::ParseArguments(argc, argv);
    if (arguments.size() <= 2) {
        throw std::invalid_argument("Not enough arguments");
    }
    return arguments;
}

Image GetImage(const std::string &path) {
    read_and_write::Reader reader(path);
    Image image = reader.ReadFile();
    return image;
}

Image ApplyFilter(const std::vector<parser::Argument> &arguments, const Image &initial_image) {
    Image image = initial_image;
    for (size_t i = 2; i < arguments.size(); ++i) {
        image = filter::MakeFilter(arguments[i])->Apply(image);
    }
    return image;
}

void WriteImage(const Image &image, const std::string &path) {
    read_and_write::Writer writer(path);
    writer.WriteImage(image);
}

void PrintHelp() {
    std::cout << "Filters:\n";
    std::cout << "  -crop width height      Crop image to width x height (e.g. 500 1000) :-P\n";
    std::cout << "  -gs                     Convert image to grayscale :*)\n";
    std::cout << "  -neg                    Apply negative filter o_0\n";
    std::cout << "  -sharp                  Sharpen image ;)\n";
    std::cout << "  -edge threshold         Edge detection with threshold (e.g. 0.1) :-/\n";
    std::cout << "  -blur sigma             Gaussian blur with given sigma (e.g. 1.5) *_*\n";
    std::cout << "  -transfer               Mimics the look of a vintage transfer effect (* ^ w ^)\n";
    std::cout << "\nExample:\n";
    std::cout << "  ./image_processor input.bmp output.bmp -transfer -gs -blur 0.5\n";
}

int main(int argc, char **argv) {
    try {
        if (argc == 1) {
            PrintHelp();
            return 0;
        }
        std::vector<parser::Argument> arguments = GetArguments(argc, argv);
        Image image = GetImage(arguments[image::utils::INPUT_FILE_POSITION].name);
        image = ApplyFilter(arguments, image);
        WriteImage(image, arguments[image::utils::OUTPUT_FILE_POSITION].name);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        PrintHelp();
        return 1;
    }
}
