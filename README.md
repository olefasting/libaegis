# libaegis

A library for building OpenCV pipelines from lambda functions.

In early development so expect breaking changes.

It currently has no documentation but here is an example:

```c++
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include "aegis/aegis.h"

using namespace aegis;

Reducer create_color_convert_reducer(const int* code) {
    return [code](InputArray input, OutputArray output) {
        cv::cvtColor(input, output, *code);
    };
}

Reducer create_gaussian_blur_reducer(const int* gauss_width, const int* gauss_height, const int* gauss_sigma_x, const int* gauss_sigma_y) {
    return [gauss_width, gauss_height, gauss_sigma_x, gauss_sigma_y](InputArray input, OutputArray output) {
        cv::GaussianBlur(input, output, cv::Size(*gauss_width, *gauss_height), *gauss_sigma_x, *gauss_sigma_y);
    };
}

Reducer create_canny_reducer(const int* canny_threshold_1, const int* canny_threshold_2) {
    return [canny_threshold_1, canny_threshold_2](InputArray input, OutputArray output) {
        cv::Canny(input, output, *canny_threshold_1, *canny_threshold_2);
    };
}

Reducer create_dilate_reducer(const int* dilate_width, const int* dilate_height) {
    return [dilate_width, dilate_height](InputArray input, OutputArray output) {
        auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(*dilate_width, *dilate_height));
        cv::dilate(input, output, kernel);
    };
}

Reducer create_erode_reducer(const int* erode_width, const int* erode_height) {
    return [erode_width, erode_height](InputArray input, OutputArray output) {
        auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(*erode_width, *erode_height));
        cv::erode(input, output, kernel);
    };
}

int main() {
int gauss_width = 3, gauss_height = 3, gauss_sigma_x = 3, gauss_sigma_y = 0;
int canny_threshold_1 = 25, canny_threshold_2 = 75;
int dilate_width = 5, dilate_height = 5;
int erode_width = 5, erode_height = 5;
auto source = CaptureSource(0);
auto pipeline = Pipeline(source, {
    create_color_convert_reducer(cv::COLOR_BGR2GRAY),
    create_gaussian_blur_reducer(&gauss_width, &gauss_height, &gauss_sigma_x, &gauss_sigma_y),
    create_canny_reducer(&canny_threshold_1, &canny_threshold_2),
    create_dilate_reducer(&dilate_width, &dilate_height),
    create_erode_reducer(&erode_width, &erode_height),
});

    Matrix output;
    auto should_quit = false;
    while (!should_quit) {
        if (pipeline.read(output)) {
            cv::imshow("Output", output);
        }
        cv::namedWindow("Settings", cv::WINDOW_AUTOSIZE);
        cv::createTrackbar("Canny Threshold 1", "Settings", &canny_threshold_1, 200);
        cv::createTrackbar("Canny Threshold 2", "Settings", &canny_threshold_2, 200);
        cv::createTrackbar("Dilate Kernel Width", "Settings", &dilate_width, 20);
        cv::createTrackbar("Dilate Kernel Height", "Settings", &dilate_height, 20);
        cv::createTrackbar("Erode Kernel Width", "Settings", &erode_width, 20);
        cv::createTrackbar("Erode Kernel Height", "Settings", &erode_height, 20);
        switch (cv::waitKey(1)) {
            case KEY_ESC:
            case KEY_Q:
                should_quit = true;
        }
    }

    return 0;
}
```

Copyright 2021 Ole A. Sjo Fasting
