# 
macro (define_dependencies_list)

    define_lib (${PROJECT_NAME} ${CMAKE_CURRENT_LIST_DIR}                          "EXELIB")

	define_lib ("TinyXML"      ${${PROJECT_NAME}_LIBS_DIR}/TinyXML    "LIB")

    if (OS_IPHONE)
        define_lib ("Foundation"    ""                                             "FRAMEWORK")
        define_lib ("UIKit"         ""                                             "FRAMEWORK")
        define_lib ("CoreGraphics"  ""                                             "FRAMEWORK")
        define_lib ("GLKit"         ""                                             "FRAMEWORK")
        define_lib ("OpenGLES"      ""                                             "FRAMEWORK")
    elseif (OS_MAC)
        define_lib ("OpenGL"        ""                                             "FRAMEWORK")
        define_lib ("Cocoa"         ""                                             "FRAMEWORK")
    endif ()

endmacro ()
