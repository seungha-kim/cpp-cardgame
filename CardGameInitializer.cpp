//
// Created by Dan Kim on 2020/10/04.
//

#include "CardGameInitializer.h"
#include <iostream>

namespace CardGame {

    // TODO: destructor

    void GraphicsContext::swapBuffer() {
        eglSwapBuffers(display, window);
    }

    GraphicsContext::GraphicsContext(EGLNativeWindowType nw) {
        nativeWindow = nw;
        // **********************
        // *** Initialize ***
        // **********************
        EGLint majorVersion;
        EGLint minorVersion;
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (display == EGL_NO_DISPLAY) {
            std::cout << "no display";
            return;
        }
        if (!eglInitialize(display, &majorVersion, &minorVersion)) {
            std::cout << "can't initialize";
            return;
        }

        // **********************
        // *** Choose config ***
        // **********************
        EGLint configAttribList[] = {
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                EGL_RED_SIZE, 5,
                EGL_GREEN_SIZE, 6,
                EGL_BLUE_SIZE, 5,
                EGL_DEPTH_SIZE, 1,
                EGL_NONE
        };
        EGLConfig configs[10];
        EGLint numConfigs;
        if (!eglChooseConfig(display, configAttribList, configs, 10, &numConfigs)) {
            std::cout << "no config";
            return;
        } else {
            std::cout << "yes config!";
        }
        config = configs[0];




        // **********************
        // *** Create surface ***
        // **********************
        window = eglCreateWindowSurface(display, config, nativeWindow, nullptr);

        if (window == EGL_NO_SURFACE) {
            std::cout << "no window";
            return;
        } else {
            std::cout << "yes surface!";
        }



        // **********************
        // *** Create context ***
        // **********************
        const EGLint contextAttribList[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2,
                EGL_NONE
        };
        context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribList);
        if (context == EGL_NO_CONTEXT) {
            std::cout << "no context";
            return;
        } else {
            std::cout << "yes context!";
        }

        // **********************
        // *** Make current ***
        // **********************
        if (!eglMakeCurrent(display, window, window, context)) {
            std::cout << "can't make current";
            return;
        }
    }

    GraphicsContext::~GraphicsContext() = default;
}