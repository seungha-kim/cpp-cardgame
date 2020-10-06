if (PP_EMSCRIPTEN)
    add_library(WebGLContext
            STATIC
            html5.h
            html5_webgl.h
            WebGLContext.h
            WebGLContext.cpp)

#
#    target_link_libraries(cardgame
#            GLESv2)
endif()
