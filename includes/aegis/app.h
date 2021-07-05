//
// Created by oasf on 04.07.2021.
//

#ifndef LIBAEGIS_APP_H
#define LIBAEGIS_APP_H

#include <vector>
#include <map>

#include <opencv2/videoio.hpp>

#include "video_source.h"

namespace aegis {
    class App {
        bool should_quit;

        std::vector<VideoSource> video_sources{};

        void handle_key_input(int key_code);

    public:
        App();

        void add_video_source(const VideoSource& capture_device);

        void run(int delay = 0);
    };
}    // namespace aegis

#endif    // LIBAEGIS_APP_H
