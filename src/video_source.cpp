//
// Created by oasf on 05.07.2021.
//

#include <utility>
#include <opencv2/videoio.hpp>

#include "aegis/video_source.h"

namespace aegis {
    VideoSource::VideoSource(const cv::VideoCapture& _capture, Pipeline  _pipeline)
            : capture(_capture), pipeline(std::move(_pipeline)) {}

    VideoSource::VideoSource(int device_index, Pipeline  _pipeline)
            : capture(cv::VideoCapture(device_index)), pipeline(std::move(_pipeline)) {}

    void VideoSource::update() {
        if (capture.grab()) {
            auto context = Context();
            capture.retrieve(context.image);
            pipeline.process(context);
            context.image.copyTo(last_frame);
        }
    }
}    // namespace aegis