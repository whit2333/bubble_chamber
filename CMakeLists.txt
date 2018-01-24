# $Id: CMakeLists.txt 86065 2014-11-07 08:51:15Z gcosmo $

#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(BUBBLESIM)

SET(BUBBLESIM_MAJOR_VERSION 0)
SET(BUBBLESIM_MINOR_VERSION 0)
SET(BUBBLESIM_PATCH_VERSION 0)
set(BUBBLESIM_VERSION ${BUBBLESIM_MAJOR_VERSION}.${BUBBLESIM_MINOR_VERSION}.${BUBBLESIM_PATCH_VERSION})

SET(SHARE_DIR_NAME bubble_sim)
SET(MACRO_DIR    share/${SHARE_DIR_NAME})
SET(GEOMETRY_DIR share/${SHARE_DIR_NAME}/geometry_files)

SET(BUBBLESIM_MACRO_DIR ${CMAKE_INSTALL_PREFIX}/${MACRO_DIR} )
SET(BUBBLESIM_GEOMETRY_DIR ${CMAKE_INSTALL_PREFIX}/${GEOMETRY_DIR} )


#set(CMAKE_CXX_FLAGS "-g3 ${CMAKE_CXX_FLAGS} ")

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# ROOT
#list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
#find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist MathMore Graf Graf3d
#   Gpad GX11 GeomPainter X3d Gviz3d RGL
#   Eve EG Foam GenVector Geom GLEW Gui HistPainter MathCore Matrix MLP Net
#   Physics Spectrum Thread Tree TreeViewer)
##find_package(ROOT REQUIRED COMPONENTS MathCore RIO Hist MathMore Graf HistPainter Rint)
#include(${ROOT_USE_FILE})
#include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIRS})
#add_definitions(${ROOT_CXX_FLAGS})


# CADMesh
find_package(cadmesh REQUIRED)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(
   ${PROJECT_SOURCE_DIR}/include
   ${PROJECT_BINARY_DIR}
   )


#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh ${PROJECT_BINARY_DIR}/BUBBLESIM.h)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
#add_executable(exampleB1 exampleB1.cc ${sources} ${headers})
#target_include_directories(exampleB1 PUBLIC ${CADMESH_INCLUDE_DIRS} )
#target_link_libraries(exampleB1 ${Geant4_LIBRARIES} ${cadmesh_LIBRARIES} )

add_executable(bubble_chamber bubble_chamber.cc ${sources} ${headers})
target_include_directories(bubble_chamber PUBLIC ${CADMESH_INCLUDE_DIRS} )
target_link_libraries(bubble_chamber ${Geant4_LIBRARIES} ${cadmesh_LIBRARIES})

#----------------------------------------------------------------------------
# For internal Geant4 use - but has no effect if you build this
# example standalone
#
add_custom_target(BUBBLESIM DEPENDS bubble_chamber)

#----------------------------------------------------------------------------
# Copy all scripts to the build directory, i.e. the directory in which we
# build B1. This is so that we can run the executable directly because it
# relies on these scripts being in the current working directory.
#
set(EXAMPLEB1_SCRIPTS
  exampleB1.in
  exampleB1.out
  examples/init_vis.mac
  examples/run1.mac
  examples/run2.mac
  examples/vis.mac
  examples/vis2.mac
  examples/trajectory_model.mac
  )
foreach(_script ${EXAMPLEB1_SCRIPTS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_script}
    ${PROJECT_BINARY_DIR}/${_script}
    COPYONLY
    )
endforeach()
install(FILES ${EXAMPLEB1_SCRIPTS} 
  DESTINATION share/${SHARE_DIR_NAME}/examples )

install(DIRECTORY  ${PROJECT_SOURCE_DIR}/geometry_files
  DESTINATION share/${SHARE_DIR_NAME} )

install(FILES
  ${PROJECT_SOURCE_DIR}/bin/run_bubble_sim 
  DESTINATION bin
  PERMISSIONS WORLD_READ WORLD_EXECUTE
  )


#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
#install(TARGETS exampleB1 DESTINATION bin)
install(TARGETS bubble_chamber DESTINATION bin)

# ----------------------------------------------------------------------------
# Configured files 
# These files end with ".in" which is removed when installed
set(EXAMPLEB1_CONF_FILES
  BubbleChamberSim.h
  init_default.mac
  #vis.mac
  )
foreach(_script ${EXAMPLEB1_CONF_FILES})
  configure_file("${_script}.in" "${PROJECT_BINARY_DIR}/${_script}" @ONLY)
  install(FILES ${PROJECT_BINARY_DIR}/${_script} DESTINATION ${CMAKE_INSTALL_PREFIX}/share/${SHARE_DIR_NAME} )
endforeach()


## Add all targets to the build-tree export set
export(TARGETS bubble_chamber FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")
#
## Export the package for use from the build-tree
## (this registers the build-tree with a global CMake-registry)
#export(PACKAGE ${PROJECT_NAME})
#
#set(CONF_LIBRARIES ${PROJECT_NAME})
#
#set(CONF_EXECUTABLES insane-config)
#
#set(CONF_INCLUDE_DIRS 
#   ${CMAKE_INSTALL_PREFIX}/include
#   ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME}
#   )
#
#set(CONF_LIBRARY_DIRS ${CMAKE_INSTALL_PREFIX}/lib)
#
##configure_file(${PROJECT_NAME}Config.cmake.in "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake" @ONLY)
#
#configure_file(${PROJECT_NAME}ConfigVersion.cmake.in "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" @ONLY)
#
## Install the FooBarConfig.cmake and FooBarConfigVersion.cmake
#install(FILES
#  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake"
#  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
#  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
#
## Install the export set for use with the install-tree
#install(EXPORT ${PROJECT_NAME}Targets DESTINATION
#  "${INSTALL_CMAKE_DIR}" COMPONENT dev)

