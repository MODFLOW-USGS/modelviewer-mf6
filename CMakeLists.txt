cmake_minimum_required(VERSION 3.20)

project (
  MvProject
  VERSION 1.0.5
  LANGUAGES CXX C
)

# set standard directory locations
include(GNUInstallDirs)

# check for vtk
find_package(VTK REQUIRED)
if(NOT VTK_DIR)
  message(FATAL_ERROR "Please set VTK_DIR.")
endif()
include(${VTK_USE_FILE})

# check for MFC
find_package(MFC REQUIRED)
if(NOT MFC_FOUND)
  message(FATAL_ERROR "MFC not found, please install Visual Studio Community 2019 / C++ MFC for latest v142 build tools (x86 & x64).")
endif()

option(MV_DEBUG_MEMORY_LEAKS "Build for memory debugging" OFF)
mark_as_advanced(MV_DEBUG_MEMORY_LEAKS)
if(MV_DEBUG_MEMORY_LEAKS)
  set(BUILD_SHARED_LIBS OFF)
else()
  set(BUILD_SHARED_LIBS ON)
endif()

add_subdirectory(mv)
add_subdirectory(ModelViewer)
