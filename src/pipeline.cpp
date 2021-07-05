//
// Created by oasf on 04.07.2021.
//

#include <opencv2/imgcodecs.hpp>

#include "aegis/pipeline.h"

namespace aegis {
    Context::Context(cv::InputArray _data) : data(_data.getMat()) {}

    Pipeline::Pipeline(std::initializer_list<Reducer> _reducers) : reducers(_reducers) {}

    Context Pipeline::process(const Context& input) const {
        Context current = input;
        for (auto& handler : reducers) {
            current = handler(current);
        }
        return current;
    }

    Reducer Pipeline::to_reducer() const {
        return [this](const Context& input) -> Context {
          return this->process(input);
        };
    }
}    // namespace aegis