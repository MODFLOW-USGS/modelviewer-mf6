cmake_minimum_required(VERSION 3.19)

project (
  ModelViewer
  VERSION ${MvProject_VERSION_MAJOR}.${MvProject_VERSION_MINOR}.${MvProject_VERSION_PATCH}
  LANGUAGES CXX
)

set(CMAKE_MFC_FLAG 2)

set(ModelViewer_COMPILE_DATE $ENV{ModelViewer_COMPILE_DATE})

configure_file(ModelViewer.rc ModelViewer.rc)
configure_file(resource.h resource.h COPYONLY)
configure_file(res/ModelViewer.ico res/ModelViewer.ico COPYONLY)
configure_file(res/MvDoc.ico res/MvDoc.ico COPYONLY)
configure_file(res/ModelViewer.rc2 res/ModelViewer.rc2 COPYONLY)

add_executable(ModelViewer WIN32
  AnimationControlsPage.cpp
  AnimationControlsPage.h
  AnimationDlg.cpp
  AnimationDlg.h
  AnimationOptionsPage.cpp
  AnimationOptionsPage.h
  # AxesDlg.cpp
  # AxesDlg.h
  AxesPage.cpp
  AxesPage.h
  BackgroundPage.cpp
  BackgroundPage.h
  BitmapResolutionDlg.cpp
  BitmapResolutionDlg.h
  BoundingBoxPage.cpp
  BoundingBoxPage.h
  ColorBarColorsPage.cpp
  ColorBarColorsPage.h
  ColorBarDataSource.cpp
  ColorBarDataSource.h
  ColorBarDlg.cpp
  ColorBarDlg.h
  ColorBarLimitsPage.cpp
  ColorBarLimitsPage.h
  ColorBarPreviewListBox.cpp
  ColorBarPreviewListBox.h
  ColorBarSizePage.cpp
  ColorBarSizePage.h
  ColorBarTextPage.cpp
  ColorBarTextPage.h
  CropControlsPage.cpp
  CropControlsPage.h
  CropDlg.cpp
  cropdlg.h
  CropOptionsPage.cpp
  CropOptionsPage.h
  CropVectorsPage.cpp
  CropVectorsPage.h
  CustomIsosurfacePage.cpp
  CustomIsosurfacePage.h
  DataDlg.cpp
  DataDlg.h
  DataFilesDialog.cpp
  DataFilesDialog.h
  DataSelectionDlg.cpp
  DataSelectionDlg.h
  DisplaySizeDlg.cpp
  DisplaySizeDlg.h
  ExportAnimationDlg.cpp
  ExportAnimationDlg.h
  GeometryDlg.cpp
  GeometryDlg.h
  GridDisplayPage.cpp
  GridDisplayPage.h
  GridDlg.cpp
  GridDlg.h
  GridLinesPage.cpp
  GridLinesPage.h
  GridShellPage.cpp
  GridShellPage.h
  ImageGenerationDlg.cpp
  ImageGenerationDlg.h
  IsosurfaceDlg.cpp
  IsosurfaceDlg.h
  LightingDlg.cpp
  lightingdlg.h
  LightsPage.cpp
  LightsPage.h
  MainFrm.cpp
  MainFrm.h
  ModelFeatureListBox.cpp
  ModelFeatureListBox.h
  ModelFeaturesDlg.cpp
  ModelFeaturesDlg.h
  ModelViewer.cpp
  ModelViewer.h
  ${PROJECT_BINARY_DIR}/ModelViewer.rc
  Modflow6DataFilesDlg.cpp
  Modflow6DataFilesDlg.h
  MvDoc.cpp
  MvDoc.h
  MvView.cpp
  MvView.h
  # MvView_x.cpp
  # MvView_x.h
  OverlayBoundsPage.cpp
  OverlayBoundsPage.h
  OverlayControlsPage.cpp
  OverlayControlsPage.h
  OverlayDlg.cpp
  OverlayDlg.h
  OverlayFilePage.cpp
  OverlayFilePage.h
  PathlineDataPage.cpp
  PathlineDataPage.h
  PathlinesClippingPage.cpp
  PathlinesClippingPage.h
  PathlinesColorPage.cpp
  PathlinesColorPage.h
  PathlinesDisplayPage.cpp
  PathlinesDisplayPage.h
  PathlinesDlg.cpp
  PathlinesDlg.h
  PreferencesDlg.cpp
  PreferencesDlg.h
  ProgressWnd.cpp
  ProgressWnd.h
  RegularIsosurfacePage.cpp
  RegularIsosurfacePage.h
  res/ModelViewer.ico
  res/ModelViewer.rc2
  res/MvDoc.ico
  resource.h
  ScalarDataPage.cpp
  ScalarDataPage.h
  ScalePage.cpp
  ScalePage.h
  SolidDlg.cpp
  SolidDlg.h
  # StdAfx.cpp
  # StdAfx.h
  SubgridPage.cpp
  SubgridPage.h
  SurfacePage.cpp
  SurfacePage.h
  VectorControlsPage.cpp
  VectorControlsPage.h
  VectorDataPage.cpp
  VectorDataPage.h
  VectorDlg.cpp
  VectorDlg.h
  VectorOptionsPage.cpp
  VectorOptionsPage.h
  VectorThresholdPage.cpp
  VectorThresholdPage.h
)

# reqdVTKLibs are the required libraries for vtk-6.0.0
# vtk-6.3.0 didn't work with this list
# now all vtk dlls copied ${VTK_LIBRARIES}
set(reqdVTKLibs
  vtkalglib
  vtkCommonComputationalGeometry
  vtkCommonCore
  vtkCommonDataModel
  vtkCommonExecutionModel
  vtkCommonMath
  vtkCommonMisc
  vtkCommonSystem
  vtkCommonTransforms
  vtkDICOMParser
  vtkexpat
  vtkFiltersCore
  vtkFiltersExtraction
  vtkFiltersGeneral
  vtkFiltersGeometry
  vtkFiltersModeling
  vtkFiltersSources
  vtkFiltersStatistics
  vtkfreetype
  vtkftgl
  vtkImagingCore
  vtkImagingFourier
  vtkImagingHybrid
  vtkInteractionStyle
  vtkIOCore
  vtkIOImage
  vtkIOXMLParser
  vtkjpeg
  vtkmetaio
  vtkpng
  vtkRenderingCore
  vtkRenderingFreeType
  vtkRenderingFreeTypeOpenGL
  vtkRenderingLOD
  vtkRenderingOpenGL
  vtksys
  vtktiff
  vtkzlib
)

# modify ModelViewer executable name
set_target_properties(ModelViewer PROPERTIES OUTPUT_NAME mvmf6)

target_include_directories(ModelViewer PRIVATE ../mv)

target_compile_definitions(ModelViewer PRIVATE _AFXDLL)

target_link_libraries(ModelViewer ${VTK_LIBRARIES})
target_link_libraries(ModelViewer mv)
target_link_libraries(ModelViewer Htmlhelp)

# converted StdAfx precompiled headers to cmake_pch
target_precompile_headers(ModelViewer
  PRIVATE
    <afxwin.h>         # MFC core and standard components
    <afxext.h>         # MFC extensions
    <afxdisp.h>        # MFC Automation classes
    <afxdtctl.h>       # MFC support for Internet Explorer 4 Common Controls
    <afxcmn.h>         # MFC support for Windows Common Controls
    <htmlhelp.h>
    <afxdlgs.h>
)

if(MSVC_VERSION EQUAL 1900)
  # set Unicode Character Set on VS2015
  target_compile_definitions(ModelViewer PRIVATE UNICODE)
  target_compile_definitions(ModelViewer PRIVATE _UNICODE)
endif()


# copy dlls into ModelViewer directory
if (MSVC)
  add_custom_command(TARGET ModelViewer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo Copying VTK dlls...
  )
  foreach(_lib ${VTK_LIBRARIES})
    add_custom_command(TARGET ModelViewer POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${_lib}> $<TARGET_FILE_DIR:ModelViewer>
    )
  endforeach()
endif()

# install
install(TARGETS ModelViewer
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# install vtk dlls
foreach(_lib ${VTK_LIBRARIES})
  install(FILES $<TARGET_FILE:${_lib}>
    DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
endforeach()

# install VS runtimes
include (InstallRequiredSystemLibraries)
