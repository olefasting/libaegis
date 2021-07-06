//
// Created by oasf on 04.07.2021.
//

#ifndef LIBAEGIS_PIPELINE_H
#define LIBAEGIS_PIPELINE_H

namespace aegis {
    typedef std::function<void(cv::InputArray, cv::OutputArray)> Reducer;

    struct Pipeline {
        std::vector<Reducer> reducers{};

        Pipeline(std::initializer_list<Reducer> _reducers = {});

        void process(cv::InputArray input, cv::OutputArray output) const;

        [[nodiscard]] Reducer to_reducer() const;
    };

    void process(const Pipeline& pipeline, cv::InputArray input, cv::OutputArray output);

    Reducer to_reducer(const Pipeline& pipeline);
}    // namespace aegis

#endif    // LIBAEGIS_PIPELINE_H
