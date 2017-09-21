file(GLOB LIB_SRC_FILES lib/*.c)
add_library(cs106 SHARED ${LIB_SRC_FILES})
target_link_libraries(cs106 espace)

find_package(OpenGL REQUIRED)

file(GLOB EXE_SRC_FILES imp/*.c)
foreach(EXE_SRC_FILE ${EXE_SRC_FILES})
    get_filename_component(EXE_DEST_FILE ${EXE_SRC_FILE} NAME_WE)
    add_executable(${EXE_DEST_FILE} ${EXE_SRC_FILE})
    target_link_libraries(${EXE_DEST_FILE} cs106 ${OPENGL_LIBRARIES}
        glut) 
endforeach()
