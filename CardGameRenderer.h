//
// Created by Dan Kim on 2020/10/03.
//

#ifndef CARDGAME_CARDGAMERENDERER_H
#define CARDGAME_CARDGAMERENDERER_H

#if PP_MACOS
#include <GLES2/gl2.h>
#elif PP_IOS
#import <OpenGLES/ES2/gl.h>
#elif PP_EMSCRIPTEN
#include "gl2.h"
#endif

namespace CardGame {
    class Renderer {
    public:
        Renderer(GLsizei width, GLsizei height);
        ~Renderer();
        void render();
    private:
        GLuint program;
        GLuint vbo{};
        GLuint ebo{};
        GLuint vertexShader;
        GLuint fragmentShader;
        GLuint texture;
        GLuint getShader(GLenum type, const GLchar *shaderSource);
        void initTexture();
    };
};

#endif //CARDGAME_CARDGAMERENDERER_H
