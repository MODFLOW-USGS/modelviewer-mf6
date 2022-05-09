cmake_minimum_required(VERSION 3.20)

project (
  MvProject
  VERSION 1.1.0
  LANGUAGES CXX C
)

# set standard directory locations
include(GNUInstallDirs)

# check for vtk
message(STATUS "VTK_DIR=${VTK_DIR}")
find_package(VTK REQUIRED)

if (${VTK_VERSION} VERSION_EQUAL "6.3")
  find_package(VTK
    COMPONENTS
      vtkalglib
      vtkCommonColor
      vtkCommonComputationalGeometry
      vtkCommonCore
      vtkCommonDataModel
      vtkCommonExecutionModel
      vtkCommonMath
      vtkCommonMisc
      vtkCommonSystem
      vtkCommonTransforms
      vtkDICOMParser
      vtkFiltersCore
      vtkFiltersExtraction
      vtkFiltersGeneral
      vtkFiltersGeometry
      vtkFiltersModeling
      vtkFiltersSources
      vtkFiltersStatistics
      vtkfreetype
      vtkftgl
      vtkGUISupportMFC
      vtkImagingCore
      vtkImagingFourier
      vtkImagingHybrid
      vtkInteractionStyle
      vtkIOCore
      vtkIOImage
      vtkjpeg
      # vtkmetaio
      vtkpng
      vtkRenderingCore
      vtkRenderingFreeType
      vtkRenderingLOD
      vtkRenderingOpenGL
      vtksys
      vtktiff
      vtkzlib
    REQUIRED
  )
endif()

if (${VTK_VERSION} VERSION_EQUAL "9.1")
  find_package(VTK
    COMPONENTS
      CommonColor
      CommonComputationalGeometry
      CommonCore
      CommonDataModel
      CommonExecutionModel
      CommonMath
      CommonMisc
      CommonSystem
      CommonTransforms
      doubleconversion
      expat
      FiltersCore
      FiltersExtraction
      FiltersGeneral
      FiltersGeometry
      FiltersModeling
      FiltersSources
      FiltersStatistics
      fmt
      freetype
      glew
      GUISupportMFC
      InteractionStyle
      IOCore
      IOLegacy
      IOXML
      IOXMLParser
      kissfft
      loguru
      lz4
      lzma
      ParallelCore
      ParallelDIY
      pugixml
      RenderingCore
      RenderingFreeType
      RenderingLOD
      RenderingOpenGL2
      RenderingUI
      vtksys  # had to add vtk
      zlib
    REQUIRED
  )
endif()

# if (${VTK_VERSION} VERSION_EQUAL "9.1")
#   # python C:\VTK-9.1.0\Utilities\Maintenance\FindNeededModules.py -s ./ModelViewer/ -s ./mv/ -j C:\VTK-9.1.0-vs2019-x64-build\modules.json
#   # find_package(VTK COMPONENTS RenderingExternal REQUIRED)
#   find_package(VTK
#     COMPONENTS
#       CommonCore
#       CommonDataModel
#       CommonExecutionModel
#       CommonTransforms
#       FiltersCore
#       FiltersExtraction
#       FiltersGeneral
#       FiltersGeometry
#       FiltersModeling
#       FiltersSources
#       RenderingCore
#       RenderingLOD
#       # These modules are suggested since they implement an existing module.
#       # Uncomment those you need.
#       FiltersParallel   # implements VTK::FiltersCore
#       InteractionStyle  # implements VTK::RenderingCore
#       RenderingFreeType # implements VTK::RenderingCore
#       RenderingOpenGL2  # implements VTK::RenderingCore
#       RenderingUI       # implements VTK::RenderingCore
#       # manually added
#       GUISupportMFC
#       CommonMath
#       CommonMisc
#       CommonColor
#       CommonComputationalGeometry
#       vtksys
#       CommonSystem
#       glew           ## vtkglew
#       loguru         ## vtkloguru
#       FiltersStatistics
#       ParallelDIY    ## vtkParallelDIY
#       ParallelCore
#       kissfft        ## vtkkissfft
#       fmt            ## vtkfmt
#       pugixml        ## vtkpugixml
#       IOXML
#       IOLegacy
#       IOCore
#       IOXMLParser
#       lz4            ## vtklz4
#       doubleconversion ## vtkdoubleconversion
#       lzma           ## vtklzma
#       zlib           ## vtkzlib
#       expat          ## vtkexpat
#       # after #include <vtkAutoInit.h>\nVTK_MODULE_INIT(vtkRenderingFreeType)
#       freetype       ## vtkfreetype
#       FiltersHybrid
#       FiltersTexture
#       ImagingSources
#       ImagingCore
#     REQUIRED
#   )
# endif()

if(NOT VTK_DIR)
  message(FATAL_ERROR "Please set VTK_DIR.")
endif()
if (NOT VTK_FOUND)
  message("${VTK_NOT_FOUND_MESSAGE}")
  return()
endif()
if (${VTK_VERSION} VERSION_LESS "9.0")
  message(STATUS "VTK_USE_FILE=${VTK_USE_FILE}")
  include(${VTK_USE_FILE})
else()
  message(STATUS "Found vtk ${VTK_VERSION}")
endif()

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

add_subdirectory(doc)
add_subdirectory(examples)
add_subdirectory(mv)
add_subdirectory(ModelViewer)
