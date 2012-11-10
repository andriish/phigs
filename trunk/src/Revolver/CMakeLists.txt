ADD_EXECUTABLE(Revolver Main.C Workstation.C)
TARGET_LINK_LIBRARIES(Revolver phigs vk
                      ${OPENGL_LIBRARY} ${X11_LIBRARY} ${XMU_LIBRARY}
                      -lXm -lGLw m
                      )

