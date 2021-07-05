//
// Created by oasf on 04.07.2021.
//

#ifndef LIBAEGIS_KEY_CODE_H
#define LIBAEGIS_KEY_CODE_H

namespace aegis {
    // FIXME: This is not portable and keycodes will vary depending on what backend is being used by highgui
    enum KeyCode {
        NONE = -1,
        ESC  = 27,
        Q    = 113,
    };
}

#endif    // LIBAEGIS_KEY_CODE_H
