cmake_minimum_required(VERSION 3.19)

project(MvProject CXX C)

# set standard directory locations
include(GNUInstallDirs)

find_package(VTK REQUIRED)
if(NOT VTK_DIR)
  message(FATAL_ERROR "Please set VTK_DIR.")
endif(NOT VTK_DIR)
include(${VTK_USE_FILE})

add_subdirectory(mv)
add_subdirectory(ModelViewer)
