cmake_minimum_required(VERSION 3.20)

project (
  mv
  VERSION ${MvProject_VERSION_MAJOR}.${MvProject_VERSION_MINOR}.${MvProject_VERSION_PATCH}
  LANGUAGES CXX C
)

add_library(mv
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
)

# generate mvHeader for dll linkage
include(GenerateExportHeader)
generate_export_header(mv
  EXPORT_FILE_NAME mvHeader.h
)

if (MV_DEBUG_MEMORY_LEAKS)
  # set for all configs
  target_compile_definitions(mv PRIVATE MV_DEBUG_MEMORY_LEAKS)
  # set only for debug config
  target_compile_definitions(mv PRIVATE $<$<CONFIG:Debug>:_CRTDBG_MAP_ALLOC>)
endif()

target_include_directories(mv PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(mv ${VTK_LIBRARIES})

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

if (MSVC AND BUILD_SHARED_LIBS)
  add_custom_command(TARGET mv POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:ModelViewer>
  )
  add_custom_command(TARGET mv POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mv> $<TARGET_FILE_DIR:ModelViewer>
  )
endif()

# install
install(TARGETS mv
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
