//
// Created by oasf on 04.07.2021.
//

#include <utility>

#include "aegis/exception.h"

namespace aegis {
    Exception::Exception(int  _code, std::string  _message) : code(_code), message(std::move(_message)) {}

    const char* Exception::what() const noexcept {
        return message.c_str();
    }
}