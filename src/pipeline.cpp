//
// Created by oasf on 04.07.2021.
//

#include <utility>

#include <opencv2/imgcodecs.hpp>

#include "aegis/pipeline.h"

namespace aegis {

    Reducer create_copy_reducer() {
        return [](cv::InputArray input, cv::OutputArray output) {
          input.copyTo(output);
        };
    }

    Reducer create_color_convert_reducer(int code) {
        return [code](cv::InputArray input, cv::OutputArray output) {
          cv::cvtColor(input, output, code);
        };
    }

    Reducer create_color_convert_reducer(const int* code) {
        return [code](cv::InputArray input, cv::OutputArray output) {
          cv::cvtColor(input, output, *code);
        };
    }

    Reducer create_gaussian_blur_reducer(const int* gauss_width, const int* gauss_height, const int* gauss_sigma_x,
                                         const int* gauss_sigma_y) {
        return [gauss_width, gauss_height, gauss_sigma_x, gauss_sigma_y](cv::InputArray input, cv::OutputArray output) {
          cv::GaussianBlur(input, output, cv::Size(*gauss_width, *gauss_height), *gauss_sigma_x, *gauss_sigma_y);
        };
    }

    Reducer create_gaussian_blur_reducer(int gauss_width, int gauss_height, int gauss_sigma_x, int gauss_sigma_y) {
        return [gauss_width, gauss_height, gauss_sigma_x, gauss_sigma_y](cv::InputArray input, cv::OutputArray output) {
          cv::GaussianBlur(input, output, cv::Size(gauss_width, gauss_height), gauss_sigma_x, gauss_sigma_y);
        };
    }

    Reducer create_canny_reducer(int canny_threshold_1, int canny_threshold_2) {
        return [canny_threshold_1, canny_threshold_2](cv::InputArray input, cv::OutputArray output) {
          cv::Canny(input, output, canny_threshold_1, canny_threshold_2);
        };
    }

    Reducer create_canny_reducer(const int* canny_threshold_1, const int* canny_threshold_2) {
        return [canny_threshold_1, canny_threshold_2](cv::InputArray input, cv::OutputArray output) {
          cv::Canny(input, output, *canny_threshold_1, *canny_threshold_2);
        };
    }

    Reducer create_dilate_reducer(int dilate_width, int dilate_height) {
        return [dilate_width, dilate_height](cv::InputArray input, cv::OutputArray output) {
          auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(dilate_width, dilate_height));
          cv::dilate(input, output, kernel);
        };
    }

    Reducer create_dilate_reducer(const int* dilate_width, const int* dilate_height) {
        return [dilate_width, dilate_height](cv::InputArray input, cv::OutputArray output) {
          auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(*dilate_width, *dilate_height));
          cv::dilate(input, output, kernel);
        };
    }

    Reducer create_erode_reducer(int erode_width, int erode_height) {
        return [erode_width, erode_height](cv::InputArray input, cv::OutputArray output) {
          auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(erode_width, erode_height));
          cv::erode(input, output, kernel);
        };
    }

    Reducer create_erode_reducer(const int* erode_width, const int* erode_height) {
        return [erode_width, erode_height](cv::InputArray input, cv::OutputArray output) {
          auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(*erode_width, *erode_height));
          cv::erode(input, output, kernel);
        };
    }

    Group::Group(std::string _name, std::initializer_list<Reducer> _reducers) : name(std::move(_name)), reducers(_reducers) {}

    void Group::process(cv::InputArray input, cv::OutputArray output) const {
        cv::Mat _input, _output;
        _input = input.getMat();
        for (const auto& reducer : reducers) {
            _output.deallocate();
            reducer(_input, _output);
            _output.copyTo(_input);
        }
        _output.copyTo(output);
    }

    Reducer Group::get_reducer() const {
        return [this](cv::InputArray input, cv::OutputArray output) {
            this->process(input, output);
        };
    }

    std::string Group::get_name() const {
        return name;
    }

    void Group::set_name(std::string _name) {
        name = std::move(_name);
    }

    Pipeline::Pipeline(std::string _name, cv::VideoCapture* _source, Reducer _reducer)
            : name(std::move(_name)), source(_source), reducer(std::move(_reducer)) {}

    Pipeline::Pipeline(cv::VideoCapture* _source, Reducer _reducer)
            : Pipeline(UNNAMED_PIPELINE, _source, std::move(_reducer)) {}

    Pipeline::Pipeline(std::string _name, cv::VideoCapture* _source, const Group& group)
            : Pipeline(std::move(_name), _source, group.get_reducer()) {}

    Pipeline::Pipeline(cv::VideoCapture* _source, const Group& group)
            : Pipeline(_source, group.get_reducer()) {}

    std::string Pipeline::get_name() {
        return name;
    }

    void Pipeline::set_name(std::string _name) {
        name = std::move(_name);
    }

    void Pipeline::get_frame(cv::OutputArray output) {
        cv::Mat input;
        source->read(input);
        reducer(input, output);
    }
}    // namespace aegis