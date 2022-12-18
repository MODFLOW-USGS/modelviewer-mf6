cmake_minimum_required(VERSION 3.21)

cmake_policy(SET CMP0057 NEW)

project (
  QMvProject${VTK_SUFFIX}
  VERSION 1.1.0
  LANGUAGES CXX C
)

set(CMAKE_CXX_STANDARD 11)

# set standard directory locations
include(GNUInstallDirs)

if(APPLE)
  set(CMAKE_INSTALL_BINDIR .)
endif()

# check for vtk
find_package(VTK REQUIRED)

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
    DICOMParser
    doubleconversion
    expat
    FiltersCore
    FiltersExtraction
    FiltersGeneral
    FiltersGeometry
    FiltersHybrid
    FiltersModeling
    FiltersSources
    FiltersStatistics
    fmt
    freetype
    glew
    # GUISupportMFC
    GUISupportQt
    ImagingCore
    ImagingSources
    InteractionStyle
    IOCore
    IOImage
    IOLegacy
    IOXML
    IOXMLParser
    jpeg
    kissfft
    loguru
    lz4
    lzma
    metaio
    ParallelCore
    ParallelDIY
    png
    pugixml
    RenderingCore
    RenderingFreeType
    RenderingLOD
    RenderingOpenGL2
    RenderingUI
    vtksys
    tiff
    zlib
  REQUIRED
)

if (NOT VTK_DIR)
  message(FATAL_ERROR "Please set VTK_DIR.")
endif()

if (NOT VTK_FOUND)
  message("${VTK_NOT_FOUND_MESSAGE}")
  return()
endif()

option(MV_DEBUG_MEMORY_LEAKS "Build for memory debugging" OFF)
mark_as_advanced(MV_DEBUG_MEMORY_LEAKS)
if (MV_DEBUG_MEMORY_LEAKS)
  set(BUILD_SHARED_LIBS OFF)
else()
  set(BUILD_SHARED_LIBS ON)
endif()

if(NOT APPLE)
  add_subdirectory(doc)
  add_subdirectory(examples)
endif()
add_subdirectory(mv)
add_subdirectory(QModelViewer)

# set default startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ModelViewer)

# based on code from CMake's QtDialog/CMakeLists.txt
macro(install_qt5_plugin _qt_plugin_name _qt_plugins_var)
  get_target_property(_qt_plugin_path "${_qt_plugin_name}" LOCATION)
  if(EXISTS "${_qt_plugin_path}")
    get_filename_component(_qt_plugin_file "${_qt_plugin_path}" NAME)
    get_filename_component(_qt_plugin_type "${_qt_plugin_path}" PATH)
    get_filename_component(_qt_plugin_type "${_qt_plugin_type}" NAME)
    if(APPLE)
      set(_qt_plugin_dest "mvmf6.app/Contents/PlugIns/${_qt_plugin_type}")
    elseif(UNIX)
      set(_qt_plugin_dest "plugins/${_qt_plugin_type}")
    elseif(WIN32)
      set(_qt_plugin_dest "bin/${_qt_plugin_type}")
    else()
      message(FATAL_ERROR "Unknown platform")
    endif()
    install(FILES "${_qt_plugin_path}"
      DESTINATION "${_qt_plugin_dest}")
    set(${_qt_plugins_var}
      "${${_qt_plugins_var}};\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${_qt_plugin_dest}/${_qt_plugin_file}")
  else()
    message(FATAL_ERROR "QT plugin ${_qt_plugin_name} not found")
  endif()
endmacro()

# set platform plugin
if(APPLE)
  install_qt5_plugin("Qt5::QCocoaIntegrationPlugin"         QT_PLUGINS)
elseif(UNIX)
  install_qt5_plugin("Qt5::QXcbIntegrationPlugin"           QT_PLUGINS)
  install_qt5_plugin("Qt5::QGtk3ThemePlugin"                QT_PLUGINS)
  install_qt5_plugin("Qt5::QIbusPlatformInputContextPlugin" QT_PLUGINS)
  install_qt5_plugin("Qt5::QXcbGlxIntegrationPlugin"        QT_PLUGINS)
  install_qt5_plugin("Qt5::QGifPlugin"                      QT_PLUGINS)
  install_qt5_plugin("Qt5::QICNSPlugin"                     QT_PLUGINS)
  install_qt5_plugin("Qt5::QICOPlugin"                      QT_PLUGINS)
  install_qt5_plugin("Qt5::QJpegPlugin"                     QT_PLUGINS)
  install_qt5_plugin("Qt5::QSvgIconPlugin"                  QT_PLUGINS)
  install_qt5_plugin("Qt5::QTgaPlugin"                      QT_PLUGINS)
  install_qt5_plugin("Qt5::QTiffPlugin"                     QT_PLUGINS)
  install_qt5_plugin("Qt5::QWbmpPlugin"                     QT_PLUGINS)
  install_qt5_plugin("Qt5::QWebpPlugin"                     QT_PLUGINS)
elseif(WIN32)
  install_qt5_plugin("Qt5::QWindowsIntegrationPlugin"       QT_PLUGINS)
endif()

# set executable
if(APPLE)
  set(APP "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/mvmf6.app")
elseif(UNIX)
  set(APP "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/bin/mvmf6")
elseif(WIN32)
  set(APP "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/bin/mvmf6.exe")
endif()

# add qt5 dir to DIRS
set(DIRS "${Qt5Widgets_DIR}")
if(NOT DIRS MATCHES "/$")
  string(APPEND DIRS "/")
endif()
if(APPLE)
  string(APPEND DIRS "../..")
elseif(UNIX)
  get_filename_component(DIRS "${DIRS}" DIRECTORY)
  get_filename_component(DIRS "${DIRS}" DIRECTORY)
elseif(WIN32)
  string(APPEND DIRS "../../../bin")
endif()

# add qt5-vtk to DIRS
if(APPLE)
  list(APPEND DIRS "${VTK_PREFIX_PATH}/lib")
elseif(UNIX)
  list(APPEND DIRS "${VTK_PREFIX_PATH}/lib")
elseif(WIN32)
  list(APPEND DIRS "${VTK_PREFIX_PATH}/bin")
endif()

# message(STATUS "CMAKE_BUNDLE_LOCATION: ${CMAKE_BUNDLE_LOCATION}")
# message(STATUS "APP: ${APP}")
# message(STATUS "QT_PLUGINS: ${QT_PLUGINS}")
# message(STATUS "DIRS: ${DIRS}")
# message(STATUS "VTK_LIBRARIES: ${VTK_LIBRARIES}")

if (APPLE OR WIN32)
  install(CODE "include(BundleUtilities)
    fixup_bundle(\"${APP}\" \"${QT_PLUGINS}\" \"${DIRS}\")")
elseif(UNIX)
  install(CODE "
    cmake_policy(SET CMP0057 NEW)  # Add support for IN_LIST operator.
    file(GET_RUNTIME_DEPENDENCIES
      EXECUTABLES $<TARGET_FILE:ModelViewer>
      LIBRARIES ${QT_PLUGINS}
      RESOLVED_DEPENDENCIES_VAR _resolves
      UNRESOLVED_DEPENDENCIES_VAR _u_deps
      CONFLICTING_DEPENDENCIES_PREFIX conflicts
      DIRECTORIES ${DIRS}
    )
    set(_dirs ${DIRS})
    foreach(_resolve \${_resolves})
      get_filename_component(_name \${_resolve} NAME)
      get_filename_component(_path \${_resolve} DIRECTORY)
      if(_path IN_LIST _dirs)
        file(INSTALL DESTINATION \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}\" TYPE SHARED_LIBRARY FOLLOW_SYMLINK_CHAIN FILES \${_resolve} )
      endif()
    endforeach()
  ")
endif()

if(APPLE)
  set(CPACK_GENERATOR "DragNDrop")
elseif(UNIX)
  set(CPACK_GENERATOR "TGZ")
elseif(WIN32)
  set(CPACK_GENERATOR "ZIP")
endif()

set(CPACK_PACKAGE_NAME "mvmf6")
SET(CPACK_PACKAGE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}${MV_VERSION_PRERELEASE}")

include(CPack)

# cmake variable debugging
if(OFF)
  get_cmake_property(_variableNames VARIABLES)
  list (SORT _variableNames)
  foreach(_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
  endforeach()
endif()
