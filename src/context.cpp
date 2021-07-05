//
// Created by oasf on 05.07.2021.
//

#include <opencv2/imgcodecs.hpp>

#include "aegis/context.h"

namespace aegis {
    Context::Context(cv::InputArray input) {
        input.copyTo(image);
    }
}