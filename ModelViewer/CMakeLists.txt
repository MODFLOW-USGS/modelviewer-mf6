cmake_minimum_required(VERSION 3.20)

project (
  ModelViewer
  VERSION ${MvProject_VERSION_MAJOR}.${MvProject_VERSION_MINOR}.${MvProject_VERSION_PATCH}
  LANGUAGES CXX
)

# This sets 'Use MFC in a Shared DLL' -- automatically sets _AFXDLL
set(CMAKE_MFC_FLAG 2)

add_executable(ModelViewer WIN32
  AnimationControlsPage.cpp
  AnimationControlsPage.h
  AnimationDlg.cpp
  AnimationDlg.h
  AnimationOptionsPage.cpp
  AnimationOptionsPage.h
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
  ModelViewer.rc
  Modflow6DataFilesDlg.cpp
  Modflow6DataFilesDlg.h
  MvDoc.cpp
  MvDoc.h
  MvView.cpp
  MvView.h
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

# modify ModelViewer executable name
set_target_properties(ModelViewer PROPERTIES OUTPUT_NAME mvmf6)

target_compile_definitions(ModelViewer PRIVATE $<$<CONFIG:Debug>:_DEBUG>)

if (MV_DEBUG_MEMORY_LEAKS)
  # set for all configs
  target_compile_definitions(ModelViewer PRIVATE MV_DEBUG_MEMORY_LEAKS)
  # set only for debug config
  target_compile_definitions(ModelViewer PRIVATE $<$<CONFIG:Debug>:_CRTDBG_MAP_ALLOC>)
endif()

target_include_directories(ModelViewer PRIVATE ${CMAKE_BINARY_DIR}/mv)
target_include_directories(ModelViewer PRIVATE ${CMAKE_SOURCE_DIR}/mv)

target_link_libraries(ModelViewer ${VTK_LIBRARIES})
target_link_libraries(ModelViewer mv)
target_link_libraries(ModelViewer Htmlhelp)

# converted StdAfx precompiled headers to cmake_pch
target_precompile_headers(ModelViewer
  PRIVATE
    # c/c++ headers
    <direct.h>
    <fstream>
    <stdio.h>
    <stdlib.h>
    <string.h>
    <time.h>
    # mfc headers
    <afxwin.h>         # MFC core and standard components
    <afxext.h>         # MFC extensions
    <afxdisp.h>        # MFC Automation classes
    <afxdtctl.h>       # MFC support for Internet Explorer 4 Common Controls
    <afxcmn.h>         # MFC support for Windows Common Controls
    <htmlhelp.h>
    <afxdlgs.h>
    # vtk headers
    <vtkCamera.h>
    <vtkInteractorStyleSwitch.h>
    <vtkLight.h>
    <vtkLightCollection.h>
    <vtkRenderer.h>
    <vtkSmartPointer.h>
    <vtkWin32OpenGLRenderWindow.h>
    <vtkWin32RenderWindowInteractor.h>
)

# if(MSVC_VERSION EQUAL 1900)
#   # set Unicode Character Set on VS2015
#   target_compile_definitions(ModelViewer PRIVATE UNICODE)
#   target_compile_definitions(ModelViewer PRIVATE _UNICODE)
# endif()

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
