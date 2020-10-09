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
        GLuint getShader(GLenum type, const GLchar *shaderSource);
    };
};

#endif //CARDGAME_CARDGAMERENDERER_H
