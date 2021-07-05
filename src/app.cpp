#include <iostream>

#include <opencv2/highgui.hpp>

#include "aegis/app.h"
#include "aegis/exception.h"
#include "aegis/key_code.h"

namespace aegis {
    App::App() : should_quit(false) {}

    void App::handle_key_input(int key_code) {
        switch (key_code) {
            case KeyCode::NONE:
                break;
            case KeyCode::Q:
            case KeyCode::ESC:
                should_quit = true;
                break;
            default:
                break;
        }
    }

    void App::add_video_source(const VideoSource& capture_device) {
        video_sources.push_back(capture_device);
    }

    void App::run(int delay) {
        try {
            should_quit = false;
            while (!should_quit) {
                for (auto& device : video_sources) {
                    device.update();
                }
                auto key = cv::waitKey(delay);
                handle_key_input(key);
            }
        } catch (cv::Exception& err) {
            throw OpenCVException(err.what());
        }
    }
}    // namespace aegis
