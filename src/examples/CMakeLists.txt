ADD_EXECUTABLE(example1 example1.c)
TARGET_LINK_LIBRARIES(example1 phigs
                      ${OPENGL_LIBRARY} ${X11_LIBRARY} Xmu m)

