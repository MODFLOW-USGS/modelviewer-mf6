cmake_minimum_required(VERSION 3.20)

project (
  mv
  # VERSION ${MvProject${VTK_SUFFIX}_VERSION_MAJOR}.${MvProject${VTK_SUFFIX}_VERSION_MINOR}.${MvProject${VTK_SUFFIX}_VERSION_PATCH}
  LANGUAGES CXX C
)

if(APPLE)
  set(mv_library_type STATIC)
else()
  set(mv_library_type SHARED)
endif() 

# add_library(mv SHARED
add_library(mv ${mv_library_type}
  Modflow6DataSource.cpp
  Modflow6DataSource.h
  ModpathReader.cpp
  ModpathReader.h
  mvAxes.cpp
  mvAxes.h
  mvBoundingBox.cpp
  mvBoundingBox.h
  mvClipBox.cpp
  mvClipBox.h
  mvColorBar.cpp
  mvColorBar.h
  mvColorTable.cpp
  mvColorTable.h
  mvCustomAppendPolyData.cpp
  mvCustomAppendPolyData.h
  mvDataSource.cpp
  mvDataSource.h
  mvDefine.h
  mvDisplayObject.cpp
  mvDisplayObject.h
  mvDisplayText.cpp
  mvDisplayText.h
  mvDxfReader.cpp
  mvDxfReader.h
  mvGridLayer.cpp
  mvGridLayer.h
  mvGridLines.cpp
  mvGridLines.h
  mvGridShell.cpp
  mvGridShell.h
  mvGUISettings.cpp
  mvGUISettings.h
  mvHashTable.cpp
  mvHashTable.h
  mvHeader.h
  mvLinkList.cpp
  mvLinkList.h
  mvLogColorTable.cpp
  mvLogColorTable.h
  mvManager.cpp
  mvManager.h
  mvModelFeatures.cpp
  mvModelFeatures.h
  mvModelList.cpp
  mvModelList.h
  mvOverlay.cpp
  mvOverlay.h
  mvPathlines.cpp
  mvPathlines.h
  mvUtil.cpp
  mvUtil.h
  shpopen.c
  shapefil.h
  ##win32/mvSaveCurrentDirectory.cpp
  ##win32/mvSaveCurrentDirectory.h
  ##qt/mvSaveCurrentDirectory.cpp
  ##qt/mvSaveCurrentDirectory.h
  mvSaveCurrentDirectory.cpp
  mvSaveCurrentDirectory.h
)

# generate mvHeader for dll linkage
include(GenerateExportHeader)
generate_export_header(mv
  EXPORT_FILE_NAME mvHeader.h
)

target_compile_definitions(mv PRIVATE $<$<CONFIG:Debug>:_DEBUG>)

# @TODO clean this up
# # # target_compile_definitions(mv PRIVATE MV_USE_VTKMFCWINDOW)
if(WIN32)
  target_compile_definitions(mv PRIVATE _WIN32_WINNT=_WIN32_WINNT_WIN7)
endif()

if (MV_DEBUG_MEMORY_LEAKS)
  # set for all configs
  target_compile_definitions(mv PRIVATE MV_DEBUG_MEMORY_LEAKS)
  if(WIN32)
    # set only for debug config
    target_compile_definitions(mv PRIVATE $<$<CONFIG:Debug>:_CRTDBG_MAP_ALLOC>)
  endif()
endif()

target_include_directories(mv PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

if (WIN32)
  target_include_directories(mv PRIVATE win32)
endif()

target_link_libraries(mv ${VTK_LIBRARIES})

if(WIN32)
  target_link_libraries(mv Shlwapi)
endif()

if(UNIX AND NOT APPLE)
  ##set_target_properties(CMAKE_SKIP_BUILD_RPATH FALSE)
  ##set_target_properties(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
  set_target_properties(mv PROPERTIES INSTALL_RPATH "\$ORIGIN")
endif()


# precompiled headers
target_precompile_headers(mv
  PRIVATE
    # Since there's only one c file we don't need
    # a precompiled header for it
    # # std c headers -- used by shpopen.c
    # "$<$<COMPILE_LANGUAGE:C>:<assert.h$<ANGLE-R>>"
    # "$<$<COMPILE_LANGUAGE:C>:<limits.h$<ANGLE-R>>"
    # "$<$<COMPILE_LANGUAGE:C>:<math.h$<ANGLE-R>>"
    # "$<$<COMPILE_LANGUAGE:C>:<stdio.h$<ANGLE-R>>"
    # "$<$<COMPILE_LANGUAGE:C>:<stdlib.h$<ANGLE-R>>"
    # "$<$<COMPILE_LANGUAGE:C>:<string.h$<ANGLE-R>>"
    # std c++ headers
    "$<$<COMPILE_LANGUAGE:CXX>:<algorithm$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<fstream$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<typeinfo$<ANGLE-R>>"
    # vtk headers
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkActor.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkActor2D.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkAlgorithm.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkAlgorithmOutput.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkAppendPolyData.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkAxes.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkBandedPolyDataContourFilter.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkCellArray.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkCellData.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkCellLocator.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkCleanPolyData.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkClipPolyData.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkConeSource.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkContourFilter.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkCubeSource.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkCutter.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkDataObject.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkDataSetAttributes.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkDataSetMapper.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkDoubleArray.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkExtractCells.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkExtractEdges.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkExtractGeometry.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkExtractGrid.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkFeatureEdges.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkGeometryFilter.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkGlyph3D.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkHedgeHog.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkHexagonalPrism.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkHexahedron.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkIdList.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkIdTypeArray.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkImplicitFunction.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkInformation.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkIntArray.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkLODActor.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkLogLookupTable.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkLookupTable.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkMaskPoints.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkOutlineSource.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkPentagonalPrism.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkPlane.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkPointData.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkPoints.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkPolyData.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkPolyDataMapper.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkPolyDataMapper2D.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkPropCollection.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkProperty.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkProperty2D.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkSmartPointer.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkStructuredGrid.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkStructuredGridGeometryFilter.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkTextMapper.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkTextProperty.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkThreshold.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkThresholdPoints.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkTransform.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkTransformPolyDataFilter.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkTubeFilter.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkUnsignedCharArray.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkUnstructuredGrid.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkViewport.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkWedge.h$<ANGLE-R>>"
    "$<$<COMPILE_LANGUAGE:CXX>:<vtkWindow.h$<ANGLE-R>>"
)

# # # if (MSVC AND BUILD_SHARED_LIBS)
# if (MSVC)
#   add_custom_command(TARGET mv POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:QModelViewer>
#   )
#   add_custom_command(TARGET mv POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mv> $<TARGET_FILE_DIR:QModelViewer>
#   )
# endif()

if(${mv_library_type} STREQUAL SHARED)
  # install
  install(TARGETS mv
    BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
endif()
