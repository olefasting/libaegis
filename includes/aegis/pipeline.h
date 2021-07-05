//
// Created by oasf on 04.07.2021.
//

#ifndef LIBAEGIS_PIPELINE_H
#define LIBAEGIS_PIPELINE_H

#include "context.h"

namespace aegis {
    typedef std::function<void(Context&)> Handler;

    class Pipeline {
        std::vector<Handler> handlers{};

    public:
        Pipeline() = default;

        void use(const Handler& handler);

        void process(Context& context);
        void process(cv::InputArray input);
        void process(cv::InputArray input, cv::OutputArray output);
    };
}    // namespace aegis

#endif    // LIBAEGIS_PIPELINE_H
