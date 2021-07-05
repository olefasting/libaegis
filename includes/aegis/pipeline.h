//
// Created by oasf on 04.07.2021.
//

#ifndef LIBAEGIS_PIPELINE_H
#define LIBAEGIS_PIPELINE_H

namespace aegis {
    struct Context {
        cv::Mat data;

        explicit Context(cv::InputArray _data = cv::noArray());
    };

    typedef std::function<Context(const Context&)> Reducer;

    class Pipeline {
        std::vector<Reducer> reducers{};

    public:
        Pipeline(std::initializer_list<Reducer> _reducers = {});

        [[nodiscard]] Context process(const Context& input) const;

        [[nodiscard]] Reducer to_reducer() const;
    };
}    // namespace aegis

#endif    // LIBAEGIS_PIPELINE_H
