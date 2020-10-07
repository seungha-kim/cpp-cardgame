//
// Created by Dan Kim on 2020/10/03.
//

#ifndef CARDGAME_CARDGAMERENDERER_H
#define CARDGAME_CARDGAMERENDERER_H

#if PP_MACOS || PP_EMSCRIPTEN
#include <GLES2/gl2.h>
#elif PP_IOS
#import <OpenGLES/ES2/gl.h>
#endif

namespace CardGameRenderer {
    void render(GLsizei width, GLsizei height);
};

#endif //CARDGAME_CARDGAMERENDERER_H
