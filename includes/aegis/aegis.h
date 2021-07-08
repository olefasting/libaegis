//
// Created by oasf on 04.07.2021.
//

#ifndef LIBAEGIS_AEGIS_H
#define LIBAEGIS_AEGIS_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>

namespace aegis {
    enum KeyCode {
        KEY_NONE = -1,
        KEY_ESC = 27,
        KEY_Q = 113,
    };

    typedef cv::Mat Matrix;
    typedef cv::InputArray InputArray;
    typedef cv::OutputArray OutputArray;

    class VideoSource {
    public:
        virtual VideoSource& operator>>(Matrix& matrix) = 0;

        virtual VideoSource& operator>>(cv::UMat& matrix) = 0;

        virtual bool read(OutputArray output) = 0;
    };

    // TODO: Implement grab and retrieve
    class CaptureSource : public VideoSource {
        cv::VideoCapture _capture;

    public:
        explicit CaptureSource(int device_index);

        explicit CaptureSource(const std::string& file_path);

        CaptureSource& operator>>(Matrix& matrix) override;

        CaptureSource& operator>>(cv::UMat& matrix) override;

        bool read(OutputArray output) override;
    };

    typedef std::function<void(InputArray, OutputArray)> Reducer;

    Reducer create_passthrough_reducer();

    Reducer create_color_convert_reducer(int code);

    Reducer create_color_convert_reducer(const int* code);

    Reducer create_gaussian_blur_reducer(int gauss_width, int gauss_height, int gauss_sigma_x, int gauss_sigma_y);

    Reducer create_gaussian_blur_reducer(const int* gauss_width, const int* gauss_height, const int* gauss_sigma_x,
                                         const int* gauss_sigma_y);

    Reducer create_canny_reducer(int canny_threshold_1, int canny_threshold_2);

    Reducer create_canny_reducer(const int* canny_threshold_1, const int* canny_threshold_2);

    Reducer create_dilate_reducer(int dilate_width, int dilate_height);

    Reducer create_dilate_reducer(const int* dilate_width, const int* dilate_height);

    Reducer create_erode_reducer(int dilate_width, int dilate_height);

    Reducer create_erode_reducer(const int* dilate_width, const int* dilate_height);

    class Pipeline : public VideoSource {
        VideoSource& _source;
        std::vector<Reducer> _reducers;

    public:
        explicit Pipeline(CaptureSource& source, std::initializer_list<Reducer> reducers = {});

        Pipeline& operator>>(Matrix& matrix) override;

        Pipeline& operator>>(cv::UMat& matrix) override;

        bool read(OutputArray output) override;
    };
}// namespace aegis

#endif// LIBAEGIS_AEGIS_H
