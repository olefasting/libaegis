//
// Created by oasf on 04.07.2021.
//

#include <utility>
#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include "aegis/aegis.h"

#define UNNAMED_PIPELINE "Unnamed Pipeline"

namespace aegis {
    VideoSource::VideoSource(int device_index) : _capture(cv::VideoCapture(device_index)) {
    }

    VideoSource::VideoSource(const std::string& file_path) : _capture(cv::VideoCapture(file_path)) {
    }

    VideoSource& VideoSource::operator>>(Matrix& matrix) {
        read(matrix);
        return *this;
    }

//    VideoSource& VideoSource::operator>>(Image& image) {
//        read(image);
//        return *this;
//    }

    bool VideoSource::read(OutputArray output) {
        return _capture.read(output);
    }

    Reducer create_passthrough_reducer() {
        return [](InputArray input, OutputArray output) {
            input.copyTo(output);
        };
    }

    Reducer create_color_convert_reducer(int code) {
        return [code](InputArray input, OutputArray output) {
            cv::cvtColor(input, output, code);
        };
    }

    Reducer create_color_convert_reducer(const int* code) {
        return [code](InputArray input, OutputArray output) {
            cv::cvtColor(input, output, *code);
        };
    }

    Reducer create_gaussian_blur_reducer(const int* gauss_width, const int* gauss_height, const int* gauss_sigma_x,
                                         const int* gauss_sigma_y) {
        return [gauss_width, gauss_height, gauss_sigma_x, gauss_sigma_y](InputArray input, OutputArray output) {
            cv::GaussianBlur(input, output, cv::Size(*gauss_width, *gauss_height), *gauss_sigma_x, *gauss_sigma_y);
        };
    }

    Reducer create_gaussian_blur_reducer(int gauss_width, int gauss_height, int gauss_sigma_x, int gauss_sigma_y) {
        return [gauss_width, gauss_height, gauss_sigma_x, gauss_sigma_y](InputArray input, OutputArray output) {
            cv::GaussianBlur(input, output, cv::Size(gauss_width, gauss_height), gauss_sigma_x, gauss_sigma_y);
        };
    }

    Reducer create_canny_reducer(int canny_threshold_1, int canny_threshold_2) {
        return [canny_threshold_1, canny_threshold_2](InputArray input, OutputArray output) {
            cv::Canny(input, output, canny_threshold_1, canny_threshold_2);
        };
    }

    Reducer create_canny_reducer(const int* canny_threshold_1, const int* canny_threshold_2) {
        return [canny_threshold_1, canny_threshold_2](InputArray input, OutputArray output) {
            cv::Canny(input, output, *canny_threshold_1, *canny_threshold_2);
        };
    }

    Reducer create_dilate_reducer(int dilate_width, int dilate_height) {
        return [dilate_width, dilate_height](InputArray input, OutputArray output) {
            auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(dilate_width, dilate_height));
            cv::dilate(input, output, kernel);
        };
    }

    Reducer create_dilate_reducer(const int* dilate_width, const int* dilate_height) {
        return [dilate_width, dilate_height](InputArray input, OutputArray output) {
            auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(*dilate_width, *dilate_height));
            cv::dilate(input, output, kernel);
        };
    }

    Reducer create_erode_reducer(int erode_width, int erode_height) {
        return [erode_width, erode_height](InputArray input, OutputArray output) {
            auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(erode_width, erode_height));
            cv::erode(input, output, kernel);
        };
    }

    Reducer create_erode_reducer(const int* erode_width, const int* erode_height) {
        return [erode_width, erode_height](InputArray input, OutputArray output) {
            auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(*erode_width, *erode_height));
            cv::erode(input, output, kernel);
        };
    }

    Pipeline::Pipeline(std::string name, VideoSource& source, std::initializer_list<Reducer> reducers)
        : _name(std::move(name)), _source(source), _reducers(reducers) {
    }

    Pipeline::Pipeline(VideoSource& source, std::initializer_list<Reducer> reducers)
        : Pipeline(UNNAMED_PIPELINE, source, reducers) {
    }

    Pipeline& Pipeline::operator>>(Matrix& matrix) {
        read(matrix);
        return *this;
    }

//    Pipeline& Pipeline::operator>>(Image& image) {
//        read(image);
//        return *this;
//    }

    std::string Pipeline::get_name() {
        return _name;
    }

    void Pipeline::set_name(std::string name) {
        _name = std::move(name);
    }

    bool Pipeline::read(OutputArray output) {
        Matrix _input, _output;
        if (_source.read(_input)) {
            auto n = _reducers.size();
            for (auto i = 0; i < n; i++) {
                _output.deallocate();
                _reducers.at(i)(_input, _output);
                if (i < n) {
                    _output.copyTo(_input);
                }
            }
            _output.copyTo(output);
            return true;
        }
        return false;
    }
}// namespace aegis
