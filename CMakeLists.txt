cmake_minimum_required(VERSION 3.20)

project (
  MvProject
  VERSION 1.0.0
  LANGUAGES CXX C
)

# set standard directory locations
include(GNUInstallDirs)

find_package(VTK REQUIRED)
if(NOT VTK_DIR)
  message(FATAL_ERROR "Please set VTK_DIR.")
endif(NOT VTK_DIR)
include(${VTK_USE_FILE})

add_subdirectory(mv)
add_subdirectory(ModelViewer)
