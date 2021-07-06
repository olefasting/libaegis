//
// Created by oasf on 04.07.2021.
//

#include <opencv2/imgcodecs.hpp>

#include "aegis/pipeline.h"

namespace aegis {
    Pipeline::Pipeline(std::initializer_list<Reducer> _reducers) : reducers(_reducers) {}

    void Pipeline::process(cv::InputArray input, cv::OutputArray output) const {
        cv::Mat _input, _output;
        input.copyTo(_input);
        for (const auto& reducer : reducers) {
            _output.deallocate();
            reducer(_input, _output);
            _output.copyTo(_input);
        }
        _output.copyTo(output);
    }

    Reducer Pipeline::to_reducer() const {
        return [this](cv::InputArray input, cv::OutputArray output) {
            this->process(input, output);
        };
    }

    void process(const Pipeline& pipeline, cv::InputArray input, cv::OutputArray output) {
        return pipeline.process(input, output);
    }

    Reducer to_reducer(const Pipeline& pipeline) {
        return pipeline.to_reducer();
    }
}    // namespace aegis