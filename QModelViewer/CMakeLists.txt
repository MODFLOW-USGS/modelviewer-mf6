cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
  cmake_policy(SET CMP0071 NEW)
endif()

if(NOT(TARGET VTK::GUISupportQt))
  message(FATAL_ERROR "ModelViewer: VTK not built with Qt support.")
endif()

if(NOT DEFINED VTK_QT_VERSION)
  set(VTK_QT_VERSION 5)
endif()

set(qt_components Core Gui Widgets)
if(${VTK_QT_VERSION} VERSION_GREATER_EQUAL 6)
  list(APPEND qt_components OpenGLWidgets)
  list(APPEND qt_components InteractionWidgets)
endif()
list(SORT qt_components)
# We have ui files, so this will also bring in the macro:
#   qt5_wrap_ui or qt_wrap_ui from Widgets.
find_package(Qt${VTK_QT_VERSION} QUIET
  REQUIRED COMPONENTS ${qt_components}
)

foreach(_qt_comp IN LISTS qt_components)
  list(APPEND qt_modules "Qt${VTK_QT_VERSION}::${_qt_comp}")
endforeach()

message(STATUS "VTK_VERSION: ${VTK_VERSION}, Qt Version: ${Qt${VTK_QT_VERSION}Widgets_VERSION}")

# Instruct CMake to run moc and uic automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(UI_FILES
  aboutdialog.ui
  animationdialog.ui
  bitmapresolutiondialog.ui
  colorbardialog.ui
  cropdialog.ui
  datadialog.ui
  dataselectiondialog.ui
  displaysizedialog.ui
  exportanimationdialog.ui
  geometrydialog.ui
  griddialog.ui
  isosurfacedialog.ui
  lightingdialog.ui
  mainwindow.ui
  modelfeaturesdialog.ui
  modflow6datafilesdialog.ui
  overlaydialog.ui
  preferencesdialog.ui
  soliddialog.ui
  vectordialog.ui
)

set(HXX_FILES
  aboutdialog.h
  animationdialog.h
  bitmapresolutiondialog.h
  colorbardialog.h
  cropdialog.h
  datadialog.h
  datafilesdialog.h
  dataselectiondialog.h
  displaysizedialog.h
  exportanimationdialog.h
  geometrydialog.h
  griddialog.h
  isosurfacedialog.h
  lightingdialog.h
  mainwindow.h
  mvdoc.h
  mvview.h
  modelfeaturesdialog.h
  modflow6datafilesdialog.h
  overlaydialog.h
  preferencesdialog.h
  qabstractview.h
  resource.h
  soliddialog.h
  vectordialog.h
)

set(CXX_FILES
  aboutdialog.cpp
  animationdialog.cpp
  bitmapresolutiondialog.cpp
  colorbardialog.cpp
  cropdialog.cpp
  datadialog.cpp
  dataselectiondialog.cpp
  datafilesdialog.cpp
  displaysizedialog.cpp
  exportanimationdialog.cpp
  geometrydialog.cpp
  griddialog.cpp
  isosurfacedialog.cpp
  lightingdialog.cpp
  main.cpp
  mainwindow.cpp
  modelfeaturesdialog.cpp
  modflow6datafilesdialog.cpp
  mvdoc.cpp
  mvview.cpp
  overlaydialog.cpp
  preferencesdialog.cpp
  qabstractview.cpp
  soliddialog.cpp
  vectordialog.cpp
)

set(QRC_FILES
  modelviewer.qrc
)

set(examples_ex-gwf-csub-p04
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.cbc
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.chd
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.csub
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.csub.obs
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.csub.obs.csv
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.dis
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.dis.grb
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.hds
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.ic
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.ims
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.lst
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.mvmf6
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.nam
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.npf
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.obs
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.oc
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.rch
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.sto
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.tdis
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/ex-gwf-csub-p04.wel
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/gwf_obs.csv
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/mfsim.lst
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-csub-p04/mfsim.nam
)

set(examples_ex-gwf-disvmesh
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-disvmesh/ex-gwf-disvmesh.cbc
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-disvmesh/ex-gwf-disvmesh.disv
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-disvmesh/ex-gwf-disvmesh.disv.grb
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-disvmesh/ex-gwf-disvmesh.ghb
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-disvmesh/ex-gwf-disvmesh.hds
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-disvmesh/ex-gwf-disvmesh.ic
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-disvmesh/ex-gwf-disvmesh.ims
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-disvmesh/ex-gwf-disvmesh.lst
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-disvmesh/ex-gwf-disvmesh.mvmf6
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-disvmesh/ex-gwf-disvmesh.nam
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-disvmesh/ex-gwf-disvmesh.npf
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-disvmesh/ex-gwf-disvmesh.oc
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-disvmesh/ex-gwf-disvmesh.rch
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-disvmesh/ex-gwf-disvmesh.tdis
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-disvmesh/mfsim.lst
  ${CMAKE_SOURCE_DIR}/examples/ex-gwf-disvmesh/mfsim.nam
)

set(doc_files
  ${CMAKE_SOURCE_DIR}/doc/ofr02-106.pdf
)

source_group("Resources"
  FILES
    ${UI_FILES}
)

configure_file(mv_version.h.in mv_version.h)

# For VTK versions greater than or equal to 8.90.0:
#  CMAKE_AUTOUIC is ON so we handle uic automatically for Qt targets.
#  CMAKE_AUTOMOC is ON so we handle moc automatically for Qt targets.

# Prevent a "command line is too long" failure in Windows.
if(WIN32)
  set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
endif()

# set icon for windows
set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/ModelViewer.rc")

set(MACOSX_BUNDLE_ICON_FILE ModelViewer.icns)

set(APP_ICON_RESOURCE_MACOS ${CMAKE_CURRENT_SOURCE_DIR}/images/ModelViewer.icns)
set_source_files_properties(${APP_ICON_RESOURCE_MACOS} PROPERTIES
       MACOSX_PACKAGE_LOCATION "Resources")

# CMAKE_AUTOMOC in ON so the MOC headers will be automatically wrapped.
add_executable(ModelViewer MACOSX_BUNDLE WIN32
  ${CXX_FILES} ${HXX_FILES} ${UI_FILES} ${QRC_FILES} ${APP_ICON_RESOURCE_WINDOWS} ${APP_ICON_RESOURCE_MACOS}
)

# modify ModelViewer executable name
set_target_properties(ModelViewer PROPERTIES OUTPUT_NAME mvmf6)

if (Qt${VTK_QT_VERSION}Widgets_VERSION VERSION_LESS "5.11.0")
  qt5_use_modules(ModelViewer ${qt_components})
else()
  target_link_libraries(ModelViewer ${qt_modules})
endif()

target_include_directories(ModelViewer PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories(ModelViewer PRIVATE ${CMAKE_BINARY_DIR}/mv)
target_include_directories(ModelViewer PRIVATE ${CMAKE_SOURCE_DIR}/mv)

target_link_libraries(ModelViewer ${VTK_LIBRARIES})
target_link_libraries(ModelViewer mv)

set_target_properties(ModelViewer PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=$(SolutionDir)\\mv\\$(Configuration);C:\\QT-VTK-9.1.0-leaks-vs2019-x64\\bin;C:\\Qt\\5.15.2\\msvc2019_64\\bin;%PATH%")

if(UNIX AND NOT APPLE)
  set_target_properties(ModelViewer PROPERTIES INSTALL_RPATH "\$ORIGIN/../lib")
endif()

# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS ModelViewer
  MODULES ${VTK_LIBRARIES}
)

# install
install(TARGETS ModelViewer
  BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(APPLE)
  target_sources(ModelViewer
    PUBLIC
      ${examples_ex-gwf-csub-p04}
  )
  set_source_files_properties(${examples_ex-gwf-csub-p04} PROPERTIES
    MACOSX_PACKAGE_LOCATION SharedSupport/examples/ex-gwf-csub-p04
  )
  target_sources(ModelViewer
    PUBLIC
      ${examples_ex-gwf-disvmesh}
  )
  set_source_files_properties(${examples_ex-gwf-disvmesh} PROPERTIES
    MACOSX_PACKAGE_LOCATION SharedSupport/examples/ex-gwf-disvmesh
  )
  
  target_sources(ModelViewer
    PUBLIC
      ${doc_files}
  )
  set_source_files_properties(${doc_files} PROPERTIES
    MACOSX_PACKAGE_LOCATION SharedSupport/doc
  )
endif()

if(WIN32 AND FALSE)
  # install vtk dlls
  foreach(_lib ${VTK_LIBRARIES})
    install(FILES $<TARGET_FILE:${_lib}>
      DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
  endforeach()
endif()

if(WIN32 AND FALSE)
  # install extra vtk dlls
  foreach(_lib VTK::InteractionWidgets VTK::RenderingContext2D VTK::FiltersTexture VTK::ImagingGeneral VTK::ImagingHybrid VTK::RenderingAnnotation VTK::ImagingColor)
    install(FILES $<TARGET_FILE:${_lib}>
      DESTINATION ${CMAKE_INSTALL_BINDIR}
    )
  endforeach()
endif()

# install VS runtimes
include(InstallRequiredSystemLibraries)

include(CTest)
add_test(NAME ModelViewer COMMAND ModelViewer)
