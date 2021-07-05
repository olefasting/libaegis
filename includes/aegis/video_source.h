//
// Created by oasf on 05.07.2021.
//

#ifndef LIBAEGIS_CAPTURE_DEVICE_H
#define LIBAEGIS_CAPTURE_DEVICE_H

#include "pipeline.h"

namespace aegis {
    class VideoSource {
        cv::Mat last_frame;
        cv::VideoCapture capture;

        Pipeline pipeline;

    public:
        explicit VideoSource(const cv::VideoCapture& _capture, Pipeline  _pipeline = Pipeline());
        explicit VideoSource(int device_index, Pipeline  _pipeline = Pipeline());

        void update();
    };
}

#endif    // LIBAEGIS_CAPTURE_DEVICE_H
