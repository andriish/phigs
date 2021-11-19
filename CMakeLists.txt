CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(PHIGS LANGUAGES C CXX Fortran)
set(PROJECT_VERSION 1.0.0)
set(QCDNUM_VERSION 1.0.0)
set(QCDNUM_VERSION_MAJOR  1)
set(QCDNUM_VERSION_MINOR  0)
set(QCDNUM_VERSION_PATCH  0)
set(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_MODULE_PATH ${PHIGS_SOURCE_DIR}/cmake/Modules)
set(OpenGL_GL_PREFERENCE "GLVND")
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(X11 REQUIRED)
FIND_PACKAGE(Motif REQUIRED)


INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}/include/phigs
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Vk
    ${CMAKE_CURRENT_SOURCE_DIR}/include/GL
    ${X11_INCLUDE_DIR}
    ${MOTIF_INCLUDE_DIR}
    )
set(libphigs_SOURCES
    ut.c ut_mat.c ut_list.c ut_htab.c ut_nset.c ut_stk.c ut_conv.c
    css_ini.c css_stb.c css_set.c css_str.c css_el.c css_inq.c css_pr.c
    css_ws.c phg.c phg_attr.c
    hdl.c err.c err_num.c psl.c
    wsb.c wsb_lut.c ws_inp.c
    wst.c wsx.c wsx_util.c wstx_ini.c wsx_inp.c
    sin.c sinq.c sin_evt.c sin_ws.c sin_cvs.c sin_val.c sin_cho.c sin_str.c
    sin_dev.c
    fnt.c fnt_mroman.c fnt_roman.c
    phg_swap.c ar.c ar_conv.c ar_hier.c ar_ops.c
    wsgl.c wsgl_attr.c wsgl_hatch.c
    wsgl_clear.c wsgl_fill.c wsgl_edge.c wsgl_marker.c wsgl_line.c wsgl_text.c
    wsgl_extattr.c wsgl_light.c
    fasd3.c wsgl_fasd3clear.c wsgl_fasd3fill.c wsgl_fasd3edge.c
    sofas3.c wsgl_sofas3clear.c wsgl_sofas3fill.c wsgl_sofas3edge.c
    cb.c cb_phg.c cb_ws.c cb_wst.c cb_str.c cb_el.c cb_extel.c cb_inp.c cb_tr.c
    cb_ar.c
    fb_phg.c fb_ws.c fb_str.c fb_el.c
    )
list(TRANSFORM libphigs_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/src/libphigs/")
ADD_LIBRARY(phigs ${libphigs_SOURCES})
TARGET_LINK_LIBRARIES(phigs ${X11_X11_LIB})

SET(Vk_SOURCES
    VkBase.C
    VkApp.C
    VkComponent.C
    VkComponentList.C
    VkMenu.C
    VkMenuAction.C
    VkMenuBar.C
    VkMenuItem.C
    VkSimpleWindow.C
    VkSubMenu.C
    VkWindow.C
    VkWidgetList.C
    VkDialogManager.C
    )
list(TRANSFORM Vk_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/src/libVk/")
ADD_LIBRARY(Vk ${Vk_SOURCES})
TARGET_LINK_LIBRARIES(Vk ${X11_Xt_LIB} ${X11_Xmu_LIB} ${MOTIF_LIBRARIES})

SET(GLw_SOURCES
    GLwDrawA.c
    GLwMDrawA.c
    )

list(TRANSFORM GLw_SOURCES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/src/libGLw/")
ADD_LIBRARY(GLw ${GLw_SOURCES})
TARGET_LINK_LIBRARIES(GLw ${X11_Xt_LIB} ${X11_Xmu_LIB} ${MOTIF_LIBRARIES})

SET(PHIGS_LIBRARIES phigs
    ${OPENGL_LIBRARY} ${X11_Xt_LIB} ${X11_Xt_LIB} ${X11_Xmu_LIB} ${MOTIF_LIBRARIES} m )

ADD_EXECUTABLE(ardisplay ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/ardisplay.c)
TARGET_LINK_LIBRARIES(ardisplay ${PHIGS_LIBRARIES})

ADD_EXECUTABLE(objdisplay ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/objdisplay.c ${CMAKE_CURRENT_SOURCE_DIR}/src/tools/glm.c)
TARGET_LINK_LIBRARIES(objdisplay ${PHIGS_LIBRARIES})


ADD_EXECUTABLE(test_c1 ${CMAKE_CURRENT_SOURCE_DIR}/src/test_c/test_c1.c ${CMAKE_CURRENT_SOURCE_DIR}/src/test_c/eldata.c)
TARGET_LINK_LIBRARIES(test_c1 ${PHIGS_LIBRARIES})

ADD_EXECUTABLE(test_c2 ${CMAKE_CURRENT_SOURCE_DIR}/src/test_c/test_c2.c ${CMAKE_CURRENT_SOURCE_DIR}/src/test_c/eldata.c)
TARGET_LINK_LIBRARIES(test_c2 ${PHIGS_LIBRARIES})

ADD_EXECUTABLE(test_c3 ${CMAKE_CURRENT_SOURCE_DIR}/src/test_c/test_c3.c ${CMAKE_CURRENT_SOURCE_DIR}/src/test_c/eldata.c )
TARGET_LINK_LIBRARIES(test_c3 ${PHIGS_LIBRARIES})

ADD_EXECUTABLE(test_c4 ${CMAKE_CURRENT_SOURCE_DIR}/src/test_c/test_c4.c ${CMAKE_CURRENT_SOURCE_DIR}/src/test_c/eldata.c )
TARGET_LINK_LIBRARIES(test_c4 ${PHIGS_LIBRARIES})

ADD_EXECUTABLE(test_c5 ${CMAKE_CURRENT_SOURCE_DIR}/src/test_c/test_c5.c)
TARGET_LINK_LIBRARIES(test_c5 ${PHIGS_LIBRARIES})

ADD_EXECUTABLE(test_c6 ${CMAKE_CURRENT_SOURCE_DIR}/src/test_c/test_c6.c)
TARGET_LINK_LIBRARIES(test_c6 ${PHIGS_LIBRARIES})

ADD_EXECUTABLE(test_c7 ${CMAKE_CURRENT_SOURCE_DIR}/src/test_c/test_c7.c)
TARGET_LINK_LIBRARIES(test_c7 ${PHIGS_LIBRARIES})

ADD_EXECUTABLE(test_c8 ${CMAKE_CURRENT_SOURCE_DIR}/src/test_c/test_c8.c)
TARGET_LINK_LIBRARIES(test_c8 ${PHIGS_LIBRARIES})

ADD_EXECUTABLE(test_c9 ${CMAKE_CURRENT_SOURCE_DIR}/src/test_c/test_c9.c)
TARGET_LINK_LIBRARIES(test_c9 ${PHIGS_LIBRARIES})

ADD_EXECUTABLE(test_c10 ${CMAKE_CURRENT_SOURCE_DIR}/src/test_c/test_c10.c)
TARGET_LINK_LIBRARIES(test_c10 ${PHIGS_LIBRARIES})

ADD_EXECUTABLE(test_f1 ${CMAKE_CURRENT_SOURCE_DIR}/src/test_f/test_f1.f)
TARGET_LINK_LIBRARIES(test_f1 ${PHIGS_LIBRARIES})


SET(REVOLVER_LIBRARIES Vk phigs
    ${OPENGL_LIBRARY} GLw  ${X11_Xt_LIB} ${X11_Xmu_LIB} ${MOTIF_LIBRARIES} m)

ADD_EXECUTABLE(Revolver.exe ${CMAKE_CURRENT_SOURCE_DIR}/src/Revolver/Main.C ${CMAKE_CURRENT_SOURCE_DIR}/src/Revolver/Workstation.C)
TARGET_LINK_LIBRARIES(Revolver.exe ${REVOLVER_LIBRARIES})

include(CMakePackageConfigHelpers)
include("GNUInstallDirs")
set(CONFIG_INSTALL_DIR ${CMAKE_INSTALL_DATADIR}/PHIGS/cmake)
configure_package_config_file(cmake/Templates/phigsConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/share/PHIGS/cmake/PHIGSConfig.cmake
  INSTALL_DESTINATION ${CONFIG_INSTALL_DIR}
  PATH_VARS CMAKE_INSTALL_INCLUDEDIR
            CMAKE_INSTALL_LIBDIR)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/share/PHIGS/cmake/PHIGSConfig-version.cmake COMPATIBILITY SameMajorVersion VERSION ${PROJECT_VERSION})

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/share/PHIGS/cmake/PHIGSConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/share/PHIGS/cmake/PHIGSConfig-version.cmake
        DESTINATION ${CONFIG_INSTALL_DIR} COMPONENT devel)


install(TARGETS phigs Vk GLw DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libs)
install(DIRECTORY include/GL  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ COMPONENT devel)
install(DIRECTORY include/phigs  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ COMPONENT devel)
install(DIRECTORY include/Vk  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ COMPONENT devel)



