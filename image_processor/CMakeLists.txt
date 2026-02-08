cmake_minimum_required(VERSION 3.17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(
        image_processor
        image_processor.cpp
        filters/filters.cpp
        filters/filters.h
        filters/gauss.cpp
        filters/gauss.h
        filters/grayscale.cpp
        filters/grayscale.h
        image/color.h
        image/image.cpp
        image/image.h
        parser/parser.cpp
        parser/parser.h
        reading_and_writing/reader.cpp
        reading_and_writing/reader.h
        reading_and_writing/writer.cpp
        reading_and_writing/writer.h
        utils.h
        image/color.cpp
        filters/negative.cpp
        filters/negative.h
        filters/crop.cpp
        filters/crop.h
        filters/edge.cpp
        filters/edge.h
        filters/sharp.cpp
        filters/sharp.h
        filters/photo_effect_transfer.cpp
        filters/photo_effect_transfer.h
        unit_tests.cpp
)
