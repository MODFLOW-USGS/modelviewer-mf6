cmake_minimum_required(VERSION 3.19)

project(mv CXX C)

add_library(mv SHARED
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

target_compile_definitions(mv PRIVATE MV_EXPORTS)
target_compile_definitions(mv PRIVATE MV_DLL)

target_link_libraries(mv ${VTK_LIBRARIES})

if (MSVC)
  add_custom_command(TARGET mv POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:ModelViewer>
  )
  add_custom_command(TARGET mv POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mv> $<TARGET_FILE_DIR:ModelViewer>
  )
endif()
