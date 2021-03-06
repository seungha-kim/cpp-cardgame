//
// Created by Dan Kim on 2020/10/03.
//

#include "CardGameRenderer.h"
#include <iostream>
#include "include/core/SkCanvas.h"
#include "include/core/SkPath.h"
#include "include/core/SkSurface.h"
#include "include/core/SkData.h"
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/GrDirectContext.h"
#include "include/gpu/GrRecordingContext.h"
#include "include/gpu/gl/GrGLInterface.h"
#if PP_MACOS
#include <GLES2/gl2.h>
#elif PP_IOS
#import <OpenGLES/ES2/gl.h>
#elif PP_EMSCRIPTEN
#include "./gl2.h"

#endif

const GLchar* vertexShaderSource = R"glsl(//precision mediump float;
attribute vec3 aPosition;
attribute vec2 aTexCoord;

varying vec2 vTexCoord;

void main() {
    gl_Position = vec4(aPosition, 1.0);
    vTexCoord = aTexCoord;
})glsl";

const GLuint kIndexOfPos = 0;
const GLsizeiptr kSizeOfPos = 3;

const GLuint kIndexOfTexCoord = 1;
const GLsizeiptr kSizeOfTexCoord = 2;

const GLchar* fragmentShaderSource = R"glsl(precision mediump float;

varying vec2 vTexCoord;
uniform sampler2D uTex;

void main() {
    gl_FragColor = texture2D(uTex, vTexCoord);
})glsl";

struct Vertex {
    GLfloat pos[3];
    GLfloat texCoord[2];
};

struct Pixel {
    GLubyte r;
    GLubyte g;
    GLubyte b;
};

namespace CardGame {
    void drawStar(SkCanvas* canvas) {
        const SkScalar scale = 128.0f;
        const SkScalar R = 0.45f * scale;
        const SkScalar TAU = 6.2831853f;
        SkPath path;
        path.moveTo(R, 0.0f);
        for (int i = 1; i < 7; ++i) {
            SkScalar theta = 3 * i * TAU / 7;
            path.lineTo(R * cos(theta), R * sin(theta));
        }
        path.close();
        SkPaint p;
        p.setAntiAlias(true);
        canvas->clear(SK_ColorWHITE);
        canvas->translate(0.5f * scale, 0.5f * scale);
        canvas->drawPath(path, p);
    }

    GLuint Renderer::getShader(GLenum type, const GLchar *shaderSource) {
        GLuint shader;
        GLint compiled;

        shader = glCreateShader(type);

        if (shader == 0) {
            std::cout << "cannot create shader";
            return 0;
        }

        glShaderSource(shader, 1, &shaderSource, nullptr);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            std::cout << "cannot compile shader";
            return 0;
        }
        return shader;
    }

    Renderer::Renderer(GLsizei width, GLsizei height) {
        glViewport(0, 0, width, height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        program = glCreateProgram();
        vertexShader = getShader(GL_VERTEX_SHADER, vertexShaderSource);
        fragmentShader = getShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);

        // glLinkProgram 이전에 호출되어야 함
        glBindAttribLocation(program, kIndexOfPos, "aPosition");
        glBindAttribLocation(program, kIndexOfTexCoord, "aTexCoord");

        glLinkProgram(program);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
//        glVertexAttrib3f(kIndexOfPos, 1.0f, 1.0f, 1.0f);
        glEnableVertexAttribArray(kIndexOfPos); // glVertexAttribXXX() 로 주어진 상수 대신 vertex array (or buffer)를 사용하겠다는 의미
        glEnableVertexAttribArray(kIndexOfTexCoord);

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        initTexture();
    }

    void Renderer::initTexture() {
        // TODO: GPU surface 시도해보기
//        https://stackoverflow.com/questions/56238188/skia-rendering-text-to-opengl-texture
//https://github.com/google/skia/blob/master/site/user/api/skcanvas_creation.md#gpu
//        const GrGLInterface* interface = nullptr;
// https://github.com/google/skia/blob/master/samplecode/SampleTextureUpload.cpp
        auto canvas = SkCanvas();
        SkImageInfo info = SkImageInfo::MakeN32Premul(128, 128);

        sk_sp<SkSurface> rasterSurface = SkSurface::MakeRasterN32Premul(128, 128);
        SkCanvas* rasterCanvas = rasterSurface->getCanvas();
        drawStar(rasterCanvas);
        auto pixmap = new SkPixmap();
        rasterSurface->peekPixels(pixmap);
        std::cout << pixmap->rowBytes() << " & " << pixmap->dimensions().fHeight << " & " << pixmap->dimensions().fWidth << std::endl;

//        auto direct = GrAsDirectContext(canvas.recordingContext()); // -> null
        std::cout << "hmm..." << std::endl;
//        SkBitmap* bitmap;
//        img->asLegacyBitmap(bitmap)
//        rasterSurface->flush(SkSurface::BackendSurfaceAccess::kPresent, GrFlushInfo());
//        auto textureHandle = rasterSurface->getBackendTexture(SkSurface::kFlushRead_BackendHandleAccess);
//        std::cout << textureHandle.isValid() << std::endl;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &texture);

        glBindTexture(GL_TEXTURE_2D, texture);

//        Pixel pixels[] = {
//                Pixel{255, 0, 0},
//                Pixel{0, 255, 0},
//                Pixel{0, 0, 255},
//                Pixel{255, 255, 0},
//        };
//
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        SkColor4f colors[16384];
        for (int i = 0; i < 128; i++) {
            for (int j = 0; j < 128; j++) {
                colors[128 * i + j] = SkColor4f::FromColor(pixmap->getColor(i, j));
            }
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGBA, GL_FLOAT, colors);
        // TODO: glTexParameter?
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    void Renderer::render() {
        Vertex vertices[] = {
                Vertex{
                        {-0.5f, -0.5f, 0.0f},
                        {0.0f, 0.0f}},
                Vertex{
                        {0.5f, -0.5f, 0.0f},
                        {1.0f, 0.0f}},
                Vertex{
                        {0.5f, 0.5f, 0.0f},
                        {1.0f, 1.0f}},
                Vertex{
                        {-0.5f, 0.5f, 0.0f},
                        {0.0f, 1.0f}},
        };


        GLushort indicies[] = {0, 1, 2,
                              2, 3, 0};

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(kIndexOfPos, kSizeOfPos, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
        glVertexAttribPointer(kIndexOfTexCoord, kSizeOfTexCoord, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texCoord));

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

        // glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(program, "uTex"), 0); // https://stackoverflow.com/questions/9661878/set-the-texture-for-by-gluniform1i

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
    }

    Renderer::~Renderer() {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ebo);
        glDeleteProgram(program);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteTextures(1, &texture);
    }
}