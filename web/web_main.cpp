//
// Created by Dan Kim on 2020/10/06.
//

// https://github.com/DanRuta/webassembly-webgl-shaders

#include "WebGLContext.h"
#include "../CardGameRenderer.h"

int main() {
    char id[] = "canvas";
    auto ctx = WebGLContext(id);
    CardGameRenderer::render(100, 100);
}