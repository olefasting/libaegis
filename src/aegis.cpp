//
// Created by oasf on 04.07.2021.
//

#include <utility>
#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

#include "aegis/aegis.h"

namespace aegis {
#define UNNAMED_PIPELINE "Unnamed Pipeline"

    Reducer create_passthrough_reducer() {
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

    Pipeline::Pipeline(std::string _name, cv::VideoCapture& _source, std::initializer_list<Reducer> _reducers)
        : name(std::move(_name)), source(_source), reducers(_reducers) {
    }

    Pipeline::Pipeline(cv::VideoCapture& _source, std::initializer_list<Reducer> _reducers)
        : Pipeline(UNNAMED_PIPELINE, _source, _reducers) {
    }

    std::string Pipeline::get_name() {
        return name;
    }

    void Pipeline::set_name(std::string _name) {
        name = std::move(_name);
    }

    void Pipeline::next_frame(cv::OutputArray output) {
        cv::Mat input, current_output;
        source.read(input);
        auto n = reducers.size();
        for (auto i = 0; i < n; i++) {
            current_output.deallocate();
            reducers.at(i)(input, current_output);
            if (i < n) {
                current_output.copyTo(input);
            }
        }
        current_output.copyTo(output);
    }
}// namespace aegis
