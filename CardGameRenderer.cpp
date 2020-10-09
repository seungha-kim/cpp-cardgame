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

const GLuint kIndexOfPos = 0;
const GLsizeiptr kSizeOfPos = 3;

const GLuint kIndexOfColor = 1;
const GLsizeiptr kSizeOfColor = 4;

const GLchar* fragmentShaderSource = R"glsl(precision mediump float;
varying lowp vec4 vColor;
void main() {
   gl_FragColor = vColor;
})glsl";

struct Vertex {
    GLfloat pos[3];
    GLfloat color[4];
};

namespace CardGame {
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
        glBindAttribLocation(program, kIndexOfColor, "aColor");

        glLinkProgram(program);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glEnableVertexAttribArray(kIndexOfPos);
        glVertexAttrib3f(kIndexOfPos, 1.0f, 1.0f, 1.0f);

        glEnableVertexAttribArray(kIndexOfColor); // glVertexAttribXXX() 로 주어진 상수 대신 vertex array (or buffer)를 사용하겠다는 의미

        glVertexAttrib4f(kIndexOfColor, 1.0f, 1.0f, 1.0f, 1.0f);

        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    }

    void Renderer::render() {
        Vertex vertices[] = {
                Vertex{
                        {-0.5f, -0.5f, 0.0f},
                        {1.0f, 0.0f, 0.0f, 1.0f}},
                Vertex{
                        {0.5f, -0.5f, 0.0f},
                        {0.0f, 1.0f, 0.0f, 1.0f}},
                Vertex{
                        {0.5f, 0.5f, 0.0f},
                        {0.0f, 0.0f, 1.0f, 1.0f}},
                Vertex{
                        {-0.5f, 0.5f, 0.0f},
                        {1.0f, 1.0f, 1.0f, 1.0f}},
        };

        const GLsizeiptr numOfVertices = 4;

        GLushort indicies[] = {0, 1, 2,
                              2, 3, 0};

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(kIndexOfPos, kSizeOfPos, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));
        glVertexAttribPointer(kIndexOfColor, kSizeOfColor, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
    }

    Renderer::~Renderer() {
        GLuint buffers[] = {vbo, ebo};
        glDeleteBuffers(2, buffers);
        glDeleteProgram(program);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }
}