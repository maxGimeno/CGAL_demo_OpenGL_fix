# Created by the script cgal_create_cmake_script
# This is the CMake script for compiling a CGAL application.

project( Empty_viewer )

cmake_minimum_required(VERSION 2.8.11)
if(POLICY CMP0043)
  cmake_policy(SET CMP0043 OLD)
endif()

find_package(CGAL COMPONENTS Qt5)

if ( CGAL_FOUND AND CGAL_Qt5_FOUND)
  include( ${CGAL_USE_FILE} )

  set(CMAKE_INCLUDE_CURRENT_DIR ON)

  FIND_PACKAGE(Qt5        REQUIRED COMPONENTS OpenGL Xml)
  find_package(QGLViewer  REQUIRED)
  find_package(OpenGL     REQUIRED)

  add_definitions(${QT_DEFINITIONS})
  add_definitions(-DQT_NO_KEYWORDS)

  include_directories( ${QGLVIEWER_INCLUDE_DIR} )
  add_definitions(${QGLVIEWER_DEFINITIONS})

  
  add_executable(emptyviewer empty_viewer.cpp)
  target_link_libraries(emptyviewer ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES}
                                     ${QT_LIBRARIES} ${QGLVIEWER_LIBRARIES}
                                     ${OPENGL_gl_LIBRARY})
  qt5_use_modules(emptyviewer Xml OpenGL)

else()

  message(STATUS "This program requires the CGAL library, "
                  "and will not be compiled.")

endif()

