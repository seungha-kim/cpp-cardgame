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


namespace CardGameRenderer {

    const GLchar vertexShaderSource[] =
            "attribute vec3 aPosition;\n"
            "void main() {\n"
            "    gl_Position = vec4(aPosition, 1.0);\n"
            "}\n";

    const GLchar fragmentShaderSource[] =
            "precision mediump float;\n"
            "void main() {\n"
            "   gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
            "}\n";

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

    void triangle(GLsizei width, GLsizei height) {
        // **********************
        // *** Draw triangle ***
        // **********************
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        GLuint program = createProgram();

        GLfloat vVertices[] = {0.0f, 0.5f, 0.0f,
                               -0.5f, -0.5f, 0.0f,
                               0.5f, -0.5f, 0.0f};
        GLshort indicies[] = {0, 1, 2};
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);
        GLuint vboIds[2];
        glGenBuffers(2, vboIds);
        glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
        glBufferData(GL_ARRAY_BUFFER, 3 * 3 * sizeof(GLfloat), vVertices, GL_STATIC_DRAW);
//
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * sizeof(GLshort), indicies, GL_STATIC_DRAW);

//        glBindBuffer(GL_ARRAY_BUFFER, 0); WebGL: INVALID_OPERATION: drawArrays: no buffer is bound to enabled attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, nullptr);
//        glUniform3f(glGetUniformLocation(program, "uColor"), 1.0, 0.0, 0.0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    class CardGameRenderer {

    };
}