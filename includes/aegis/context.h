//
// Created by oasf on 05.07.2021.
//

#ifndef LIBAEGIS_CONTEXT_H
#define LIBAEGIS_CONTEXT_H

namespace aegis {
    struct Context {
        cv::Mat image;

        explicit Context() = default;
        explicit Context(cv::InputArray input);
    };
}

#endif    // LIBAEGIS_CONTEXT_H
