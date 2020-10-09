//
// Created by Dan Kim on 2020/10/06.
//

// https://github.com/DanRuta/webassembly-webgl-shaders

#include "WebGLContext.h"
#include "../CardGameRenderer.h"

int main() {
    char id[] = "canvas";
    auto ctx = WebGLContext(id);
    auto renderer = CardGame::Renderer(100, 100);
    renderer.render();
}