//
// Created by Dan Kim on 2020/10/04.
//

#ifndef CARDGAME_CARDGAMEINITIALIZER_H
#define CARDGAME_CARDGAMEINITIALIZER_H

#include <EGL/egl.h>


#include <memory>

namespace CardGame {
    class GraphicsContext {
    public:
        explicit GraphicsContext(EGLNativeWindowType nativeWindow);
        ~GraphicsContext();
        void swapBuffer();
    private:
        EGLNativeWindowType nativeWindow;
        EGLDisplay display;
        EGLConfig config;
        EGLSurface window;
        EGLContext context;
    };
}


#endif //CARDGAME_CARDGAMEINITIALIZER_H
