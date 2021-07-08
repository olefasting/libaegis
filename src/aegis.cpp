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
    CaptureSource::CaptureSource(int device_index) : _capture(cv::VideoCapture(device_index)) {
    }

    CaptureSource::CaptureSource(const std::string& file_path) : _capture(cv::VideoCapture(file_path)) {
    }

    CaptureSource& CaptureSource::operator>>(Matrix& matrix) {
        read(matrix);
        return *this;
    }

    CaptureSource& CaptureSource::operator>>(cv::UMat& matrix) {
        read(matrix);
        return *this;
    }

    bool CaptureSource::read(OutputArray output) {
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

    Reducer create_gaussian_blur_reducer(const int* width, const int* height, const int* sigma_x,
                                         const int* sigma_y) {
        return [width, height, sigma_x, sigma_y](InputArray input, OutputArray output) {
            cv::GaussianBlur(input, output, cv::Size(*width, *height), *sigma_x, *sigma_y);
        };
    }

    Reducer create_gaussian_blur_reducer(int width, int height, int sigma_x, int sigma_y) {
        return create_gaussian_blur_reducer(&width, &height, &sigma_x, &sigma_y);
    }

    Reducer create_canny_reducer(const int* threshold_1, const int* threshold_2) {
        return [threshold_1, threshold_2](InputArray input, OutputArray output) {
            cv::Canny(input, output, *threshold_1, *threshold_2);
        };
    }

    Reducer create_canny_reducer(int threshold_1, int threshold_2) {
        return create_canny_reducer(&threshold_1, &threshold_2);
    }

    Reducer create_dilate_reducer(const int* width, const int* height) {
        return [width, height](InputArray input, OutputArray output) {
            auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(*width, *height));
            cv::dilate(input, output, kernel);
        };
    }

    Reducer create_dilate_reducer(int width, int height) {
        return create_dilate_reducer(&width, &height);
    }

    Reducer create_erode_reducer(const int* width, const int* height) {
        return [width, height](InputArray input, OutputArray output) {
            auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(*width, *height));
            cv::erode(input, output, kernel);
        };
    }

    Reducer create_erode_reducer(int width, int height) {
        return create_erode_reducer(&width, &height);
    }

    Pipeline::Pipeline(CaptureSource& source, std::initializer_list<Reducer> reducers)
        : _source(source), _reducers(reducers) {
    }

    Pipeline& Pipeline::operator>>(Matrix& matrix) {
        read(matrix);
        return *this;
    }

    Pipeline& Pipeline::operator>>(cv::UMat& image) {
        read(image);
        return *this;
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
