//
// Created by oasf on 04.07.2021.
//

#ifndef LIBAEGIS_EXCEPTION_H
#define LIBAEGIS_EXCEPTION_H

#include <string>

namespace aegis {
    struct Exception : public std::exception {
        int code;
        std::string message;

        Exception(int _code, std::string  _message);

        [[nodiscard]] const char* what() const noexcept override;
    };

#define EXCEPTION_OPENCV 1

    struct OpenCVException : public Exception {
        explicit OpenCVException(std::string _message);
    };
}

#endif    // LIBAEGIS_EXCEPTION_H
