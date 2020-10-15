//https://github.com/DanRuta/webassembly-webgl-shaders/tree/master/dev/cpp

#include "WebGLContext.h"

WebGLContext::WebGLContext(char * id) {
    // Context configurations
    EmscriptenWebGLContextAttributes attrs;
    attrs.explicitSwapControl = 0;
    attrs.depth = 1;
    attrs.stencil = 1;
    attrs.antialias = 1;
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;

    context = emscripten_webgl_create_context(id, &attrs);
    emscripten_webgl_make_context_current(context);
    emscripten_webgl_enable_extension(context, "OES_texture_float");
    emscripten_webgl_enable_extension(context, "OES_texture_float_linear");
}

WebGLContext::~WebGLContext (void) {
    emscripten_webgl_destroy_context(context);
}
