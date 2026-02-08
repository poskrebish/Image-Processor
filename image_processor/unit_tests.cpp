#include <iostream>
#include <stdexcept>
#include <cassert>
#include "parser/parser.h"
#include "filters/filters.h"
#include "image/image.h"
#include "reading_and_writing/reader.h"
#include "reading_and_writing/writer.h"
#include "filters/photo_effect_transfer.h"

void TestFilters() {
    using parser::Argument;

    try {
        filter::MakeFilter(Argument{"-crop", {"1000"}});
        assert(false);
    } catch (...) {
    }

    try {
        filter::MakeFilter(Argument{"-crop", {}});
        assert(false);
    } catch (...) {
    }

    try {
        filter::MakeFilter(Argument{"-edge", {}});
        assert(false);
    } catch (...) {
    }

    try {
        filter::MakeFilter(Argument{"-blur", {"a"}});
        assert(false);
    } catch (...) {
    }

    try {
        filter::MakeFilter(Argument{"-blur", {"1", "2"}});
        assert(false);
    } catch (...) {
    }

    try {
        filter::MakeFilter(Argument{"-neg", {"1"}});
        assert(false);
    } catch (...) {
    }

    try {
        filter::MakeFilter(Argument{"-transfer", {"1"}});
        assert(false);
    } catch (...) {
    }

    try {
        filter::MakeFilter(Argument{"-nofiltr", {}});
        assert(false);
    } catch (...) {
    }
}

void TestReaderThrows() {
    try {
        read_and_write::Reader reader("test_script/data/nonexistent.bmp");
        reader.ReadFile();
        assert(false);
    } catch (...) {
    }
}

void TestWriterReaderWriter() {
    read_and_write::Reader reader("test_script/data/input.bmp");
    Image image = reader.ReadFile();

    read_and_write::Writer writer("C++/output.bmp");
    writer.WriteImage(image);

    read_and_write::Reader reader2("C++/output.bmp");
    Image read2 = reader2.ReadFile();

    assert(image.GetWidth() == read2.GetWidth() && image.GetHeight() == read2.GetHeight());
}

void TestImageAccess() {
    Image img(image::utils::TESTR_TEN, image::utils::TESTR_TEN);
    img.SetColor(image::utils::TESTR_FIVE, image::utils::TESTR_FIVE,
                 Color(image::utils::TESTR_ONE, image::utils::TESTR_ONE, image::utils::TESTR_ONE));
    Color c = img.GetColor(image::utils::TESTR_FIVE, image::utils::TESTR_FIVE);
    assert(c.red == image::utils::TESTR_ONE && c.green == image::utils::TESTR_ONE && c.blue == image::utils::TESTR_ONE);

    try {
        img.GetColor(image::utils::TESTR_FIVETEEN, image::utils::TESTR_FIVETEEN);
        assert(false);
    } catch (...) {
    }
}

void TestParser() {
    char arg0[] = "image_processor";
    char arg1[] = "input.bmp";
    char arg2[] = "output.bmp";
    char arg3[] = "-sharp";
    char arg4[] = "-edge";
    char arg5[] = "50";
    char arg6[] = "-crop";
    char arg7[] = "100";
    char arg8[] = "200";
    char arg9[] = "-neg";

    char* argv[] = {arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9};
    int argc = sizeof(argv) / sizeof(argv[0]);

    auto args = parser::ParseArguments(argc, argv);

    assert(args[0].name == "input.bmp");
    assert(args[1].name == "output.bmp");

    assert(args[2].name == "-sharp");
    assert(args[2].parameters.empty());

    assert(args[3].name == "-edge");
    assert(args[3].parameters == std::vector<std::string>({"50"}));

    assert(args[4].name == "-crop");
    assert(args[4].parameters == std::vector<std::string>({"100", "200"}));

    assert(args[5].name == "-neg");
    assert(args[5].parameters.empty());
}

void TestPhotoEffectTransfer() {
    const std::string input_path = "test_script/data/lenna.bmp";
    const std::string expected_path = "test_script/data/lenna_transfer.bmp";

    read_and_write::Reader reader_input(input_path);
    read_and_write::Reader reader_expected(expected_path);

    Image input = reader_input.ReadFile();
    Image expected = reader_expected.ReadFile();

    filter::PhotoEffectTransfer filter;
    Image result = filter.Apply(input);

    for (size_t y = 0; y < result.GetHeight(); ++y) {
        for (size_t x = 0; x < result.GetWidth(); ++x) {
            const Color& actual = result.GetColor(x, y);
            const Color& expect = expected.GetColor(x, y);
            assert(actual.red == expect.red && actual.green == expect.green && actual.blue == expect.blue);
        }
    }
}

int Tests() {
    TestFilters();
    TestReaderThrows();
    TestWriterReaderWriter();
    TestImageAccess();
    TestParser();
    TestPhotoEffectTransfer();

    std::cout << "Passed";
    return 0;
}