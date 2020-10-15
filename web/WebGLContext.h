//https://github.com/DanRuta/webassembly-webgl-shaders/tree/master/dev/cpp


#ifndef CARDGAME_WEBGLCONTEXT_H
#define CARDGAME_WEBGLCONTEXT_H

#include "../gl2.h"
#include "html5.h"


class WebGLContext {
public:
    explicit WebGLContext (char * id);

    ~WebGLContext (void);

//    void run (uint8_t* buffer);

private:
    GLuint programObject;
    GLuint vertexShader;
    GLuint fragmentShader;

    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context;

};

#endif //CARDGAME_WEBGLCONTEXT_H
