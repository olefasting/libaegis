//
// Created by oasf on 04.07.2021.
//

#include <opencv2/imgcodecs.hpp>

#include "aegis/pipeline.h"

namespace aegis {
    HandlerFunc Handler::get_func() {
        return [this](Context& context) {
            handle(context);
        };
    }

    void Pipeline::use(const Handler& handler) {
        handlers.push_back(handler);
    }

    void Pipeline::process(Context& context) {
        for (auto& handler : handlers) {
            handler(context);
        }
    }

    void Pipeline::process(cv::InputArray input) {
        auto context = Context(input);
        process(context);
    }

    void Pipeline::process(cv::InputArray input, cv::OutputArray output) {
        auto context = Context(input);
        process(context);
        context.image.copyTo(output);
    }
}