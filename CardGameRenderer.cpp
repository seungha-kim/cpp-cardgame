//
// Created by Dan Kim on 2020/10/03.
//

#include "CardGameRenderer.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <iostream>

namespace CardGameRenderer {

    const GLchar vertexShaderSource[] =
            "attribute vec3 aPosition;\n"
            "void main() {\n"
            "    gl_Position = vec4(aPosition, 1.0);\n"
            "}\n";

    const GLchar fragmentShaderSource[] =
            "precision mediump float;\n"
            "void main() {\n"
            "   gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);\n"
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

    void triangle(EGLNativeWindowType nativeWindow, GLsizei width, GLsizei height) {
        // **********************
        // *** Initialize ***
        // **********************
        EGLint majorVersion;
        EGLint minorVersion;
        EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (display == EGL_NO_DISPLAY) {
            std::cout << "no display";
            return;
        }
        if (!eglInitialize(display, &majorVersion, &minorVersion)) {
            std::cout << "can't initialize";
            return;
        }



        // **********************
        // *** Choose config ***
        // **********************
        EGLint configAttribList[] = {
                EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
                EGL_RED_SIZE, 5,
                EGL_GREEN_SIZE, 6,
                EGL_BLUE_SIZE, 5,
                EGL_DEPTH_SIZE, 1,
                EGL_NONE
        };
        EGLConfig configs[10];
        EGLint numConfigs;
        if (!eglChooseConfig(display, configAttribList, configs, 10, &numConfigs)) {
            std::cout << "no config";
            return;
        } else {
            std::cout << "yes config!";
        }
        EGLConfig config = configs[0];




        // **********************
        // *** Create surface ***
        // **********************
        EGLSurface window = eglCreateWindowSurface(display, config, nativeWindow, nullptr);

        if (window == EGL_NO_SURFACE) {
            std::cout << "no window";
            return;
        } else {
            std::cout << "yes surface!";
        }



        // **********************
        // *** Create context ***
        // **********************
        const EGLint contextAttribList[] = {
                EGL_CONTEXT_CLIENT_VERSION, 2,
                EGL_NONE
        };
        EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribList);
        if (context == EGL_NO_CONTEXT) {
            std::cout << "no context";
            return;
        } else {
            std::cout << "yes context!";
        }

        // **********************
        // *** Make current ***
        // **********************
        if (!eglMakeCurrent(display, window, window, context)) {
            std::cout << "can't make current";
            return;
        }


        // **********************
        // *** Draw triangle ***
        // **********************
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        GLuint program = createProgram();

        GLfloat vVertices[] = {0.0f, 0.5f, 0.0f,
                               -0.5f, -0.5f, 0.0f,
                               0.5f, -0.5f, 0.0f};
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, vVertices);
        glUniform3f(glGetUniformLocation(program, "uColor"), 1.0, 0.0, 0.0);
        glEnableVertexAttribArray(0);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        eglSwapBuffers(display, window);
    }

    class CardGameRenderer {

    };
}