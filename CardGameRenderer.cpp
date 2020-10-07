//
// Created by Dan Kim on 2020/10/03.
//

#include "CardGameRenderer.h"
#include <iostream>

#if PP_MACOS || PP_EMSCRIPTEN
#include <GLES2/gl2.h>
#elif PP_IOS
#import <OpenGLES/ES2/gl.h>
#endif

const GLchar* vertexShaderSource = R"glsl(//precision mediump float;
attribute vec3 aPosition; // 0
attribute vec4 aColor; // 1

varying vec4 vColor;

void main() {
    gl_Position = vec4(aPosition, 1.0);
    vColor = aColor;
})glsl";

const unsigned int kIndexOfPos = 0;
const unsigned int kOffsetOfPos = 0;
const int kSizeOfPos = 3;

const unsigned int kIndexOfColor = 1;
const GLuint kOffsetOfColor = 3;
const int kSizeOfColor = 4;
const unsigned int kBytesOfAttribs = sizeof(GLfloat) * (kSizeOfPos + kSizeOfColor);

const GLchar* fragmentShaderSource = R"glsl(precision mediump float;
varying lowp vec4 vColor;
void main() {
   gl_FragColor = vColor;
})glsl";

namespace CardGameRenderer {

    GLuint getShader(GLenum type, const GLchar *shaderSource) {
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

    GLuint createProgram() {
        GLuint program = glCreateProgram();
        GLuint vertexShader = getShader(GL_VERTEX_SHADER, vertexShaderSource);
        GLuint fragmentShader = getShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);
        return program;
    }

    void render(GLsizei width, GLsizei height) {
        glViewport(0, 0, width, height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // TODO: move this
        GLuint program = createProgram();
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);

        GLfloat vVertices[] = {
                -0.5f, -0.5f, 0.0f, // coord
                1.0f, 0.0f, 0.0f, 1.0f, // color

                0.5f, -0.5f, 0.0f,
                0.0f, 1.0f, 0.0f, 1.0f,

                0.5f, 0.5f, 0.0f,
                0.0f, 0.0f, 1.0f, 1.0f,

                -0.5f, 0.5f, 0.0f,
                1.0f, 1.0f, 0.0f, 1.0f};
        const int numOfVertices = 4;


        GLushort indicies[] = {0, 1, 2,
                              0, 2, 3};


        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, numOfVertices * kBytesOfAttribs, vVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(kIndexOfPos);
        glVertexAttribPointer(kIndexOfPos, kSizeOfPos, GL_FLOAT, GL_FALSE, kBytesOfAttribs, nullptr);
        glBindAttribLocation(program, kIndexOfPos, "aPosition");

        glEnableVertexAttribArray(kIndexOfColor); // glVertexAttribXXX() 로 주어진 상수 대신 vertex array (or buffer)를 사용하겠다는 의미
        glVertexAttribPointer(kIndexOfColor, kSizeOfColor, GL_FLOAT, GL_FALSE, kBytesOfAttribs, (const void*) (sizeof(GLfloat) * kOffsetOfColor));
//        glVertexAttrib4f(kIndexOfColor, 1.0f, 1.0f, 0.0f, 1.0f);
        glBindAttribLocation(program, kIndexOfColor, "aColor");

        GLuint ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLushort), indicies, GL_STATIC_DRAW);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
        // TODO: glDeleteBuffers()
    }

    class CardGameRenderer {

    };
}